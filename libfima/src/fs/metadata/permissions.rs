use std::{
    io,
    path::{Path, PathBuf},
};

#[cfg(unix)]
use std::os::unix::fs::PermissionsExt;

#[cfg(windows)]
use std::ptr;

#[cfg(windows)]
use crate::fs::windows;

#[cfg(windows)]
use windows::Win32::Security::{
    ACCESS_ALLOWED_ACE, ACCESS_ALLOWED_ACE_TYPE, ACCESS_DENIED_ACE, ACCESS_DENIED_ACE_TYPE,
    ACE_HEADER, ACL, DACL_SECURITY_INFORMATION, GetAce, INHERITED_ACE,
};

#[cfg(windows)]
use std::os::windows::fs::MetadataExt;

/// Represents a Windows access-control entry.
#[derive(Debug)]
pub struct WindowsPermission {
    /// The user or group this permission applies to.
    pub identity: String,

    /// Whether this permission is inherited from a parent directory.
    pub inherited: bool,

    /// Whether this entry grants or denies the specified permissions.
    pub allow: bool,

    /// The original Windows access rights mask.
    pub access_mask: u32,

    /// Whether this entry includes read access.
    pub read: bool,

    /// Whether this entry includes write access.
    pub write: bool,

    /// Whether this entry includes modification access.
    pub modify: bool,

    /// Whether this entry includes read and execute access.
    pub read_and_execute: bool,

    /// Whether this entry includes directory listing access.
    pub list_folder: bool,

    /// Whether this entry includes full control.
    pub full_control: bool,
}

#[cfg(windows)]
impl WindowsPermission {
    /// Creates a permission entry from a Windows access-control entry.
    pub fn new(
        sid: windows::Win32::Security::PSID,
        access_mask: u32,
        ace_flags: u8,
        allow: bool,
        is_directory: bool,
    ) -> io::Result<Self> {
        const GENERIC_READ: u32 = 0x8000_0000;
        const GENERIC_WRITE: u32 = 0x4000_0000;
        const GENERIC_EXECUTE: u32 = 0x2000_0000;
        const GENERIC_ALL: u32 = 0x1000_0000;

        const FILE_READ_DATA: u32 = 0x0001;
        const FILE_WRITE_DATA: u32 = 0x0002;
        const FILE_APPEND_DATA: u32 = 0x0004;
        const FILE_READ_EA: u32 = 0x0008;
        const FILE_WRITE_EA: u32 = 0x0010;
        const FILE_EXECUTE: u32 = 0x0020;
        const FILE_READ_ATTRIBUTES: u32 = 0x0080;
        const FILE_WRITE_ATTRIBUTES: u32 = 0x0100;

        const DELETE: u32 = 0x0001_0000;
        const FILE_ALL_ACCESS: u32 = 0x001F_01FF;

        let read = access_mask
            & (GENERIC_READ | FILE_READ_DATA | FILE_READ_EA | FILE_READ_ATTRIBUTES)
            != 0;

        let write = access_mask
            & (GENERIC_WRITE
                | FILE_WRITE_DATA
                | FILE_APPEND_DATA
                | FILE_WRITE_EA
                | FILE_WRITE_ATTRIBUTES)
            != 0;

        let execute = access_mask & (GENERIC_EXECUTE | FILE_EXECUTE) != 0;

        let full_control =
            access_mask & GENERIC_ALL != 0 || access_mask & FILE_ALL_ACCESS == FILE_ALL_ACCESS;

        let modify = full_control || (read && write && execute && access_mask & DELETE == 0);

        let identity = windows::sid_name(sid)
            .map_err(|error| io::Error::new(io::ErrorKind::Other, error.to_string()))?;

        Ok(Self {
            identity,
            inherited: ace_flags & INHERITED_ACE != 0,
            allow,
            access_mask,
            read,
            write,
            modify,
            read_and_execute: read && execute,
            list_folder: is_directory && access_mask & FILE_READ_DATA != 0,
            full_control,
        })
    }
}

/// Represents the permissions of a file or directory.
///
/// Unix and Windows represent permissions differently.
#[derive(Debug)]
pub enum Permissions {
    /// Unix-style permissions. Example: `rwxr-xr-x`.
    Unix {
        /// Whether the file owner can read the file.
        user_read: bool,
        /// Whether the file owner can write to the file.
        user_write: bool,
        /// Whether the file owner can execute the file.
        user_exec: bool,

        /// Whether users in the file's group can read the file.
        group_read: bool,
        /// Whether users in the file's group can write to the file.
        group_write: bool,
        /// Whether users in the file's group can execute the file.
        group_exec: bool,

        /// Whether other users can read the file.
        other_read: bool,
        /// Whether other users can write to the file.
        other_write: bool,
        /// Whether other users can execute the file.
        other_exec: bool,
    },

    /// Windows-style permissions, represented as access-control entries.
    Windows {
        /// The access-control entries associated with the file or directory.
        permissions: Vec<WindowsPermission>,

        /// The path of the item that was analyzed for the permissions, it is used for when displaying
        /// with the `mode` output
        path: PathBuf,
    },
}

impl Permissions {
    /// Gets the permissions of a file as a [`Permissions`].
    pub fn new<P>(path: P) -> io::Result<Permissions>
    where
        P: AsRef<Path>,
    {
        let path = crate::fs::utility::expand_path(path);

        #[cfg(windows)]
        {
            let (_, _, dacl, security_descriptor) =
                windows::security_info(&path, DACL_SECURITY_INFORMATION)
                    .map_err(|error| io::Error::new(io::ErrorKind::Other, error.to_string()))?;

            let _guard = windows::SecurityDescriptorGuard(security_descriptor);

            let mut permissions = Vec::new();

            if !dacl.is_null() {
                let dacl: *mut ACL = dacl;
                let ace_count = unsafe { (*dacl).AceCount };

                for index in 0..u32::from(ace_count) {
                    let mut ace = ptr::null_mut();

                    unsafe {
                        GetAce(dacl, index, &mut ace).map_err(|error| {
                            io::Error::new(io::ErrorKind::Other, error.to_string())
                        })?;
                    }

                    if ace.is_null() {
                        continue;
                    }

                    let header = unsafe { &*(ace as *const ACE_HEADER) };

                    let (sid, access_mask, allow) = unsafe {
                        match header.AceType {
                            ACCESS_ALLOWED_ACE_TYPE => {
                                let entry = &*(ace as *const ACCESS_ALLOWED_ACE);

                                (
                                    ptr::addr_of!(entry.SidStart) as windows::Win32::Security::PSID,
                                    entry.Mask,
                                    true,
                                )
                            }

                            ACCESS_DENIED_ACE_TYPE => {
                                let entry = &*(ace as *const ACCESS_DENIED_ACE);

                                (
                                    ptr::addr_of!(entry.SidStart) as windows::Win32::Security::PSID,
                                    entry.Mask,
                                    false,
                                )
                            }

                            // Other ACE types require dedicated parsing.
                            _ => continue,
                        }
                    };

                    permissions.push(WindowsPermission::new(
                        sid,
                        access_mask,
                        header.AceFlags,
                        allow,
                        path.is_dir(),
                    )?);
                }
            }

            Ok(Permissions::Windows { path, permissions })
        }

        #[cfg(unix)]
        {
            let mode = path.metadata()?.permissions().mode();

            Ok(Permissions::Unix {
                user_read: mode & 0o400 != 0,
                user_write: mode & 0o200 != 0,
                user_exec: mode & 0o100 != 0,
                group_read: mode & 0o040 != 0,
                group_write: mode & 0o020 != 0,
                group_exec: mode & 0o010 != 0,
                other_read: mode & 0o004 != 0,
                other_write: mode & 0o002 != 0,
                other_exec: mode & 0o001 != 0,
            })
        }
    }

    /// Display the permissions into a string.
    ///
    /// - Unix: `rw-r--r--` (the file type is omitted).
    /// - Windows: one access-control entry per line or the mode
    ///
    /// # Arguments
    ///
    /// - `mode` - Display the mode (like the dir command), this is created for compatibility with
    ///            `ls -l`
    /// - `path` - Used for when you use the `mode` output
    pub fn display(&self, _mode: bool) -> anyhow::Result<String> {
        let mut string = String::new();

        match self {
            Permissions::Unix {
                user_read,
                user_write,
                user_exec,
                group_read,
                group_write,
                group_exec,
                other_read,
                other_write,
                other_exec,
            } => {
                // We don't add the file type as it isn't actually part of the permissions

                for (enabled, character) in [
                    (user_read, 'r'),
                    (user_write, 'w'),
                    (user_exec, 'x'),
                    (group_read, 'r'),
                    (group_write, 'w'),
                    (group_exec, 'x'),
                    (other_read, 'r'),
                    (other_write, 'w'),
                    (other_exec, 'x'),
                ] {
                    string.push(if *enabled { character } else { '-' });
                }
            }
            Permissions::Windows {
                permissions: _permissions,
                path: _path,
            } => {
                #[cfg(windows)]
                if _mode {
                    // We have this option because it's the best for an ls -l like command, as every
                    // file has multiple users permissions
                    let attributes = _path.symlink_metadata()?.file_attributes();

                    string.push(if attributes & 0x10 != 0 { 'd' } else { '-' }); // Directory
                    string.push(if attributes & 0x01 != 0 { 'r' } else { '-' }); // Read-only
                    string.push(if attributes & 0x02 != 0 { 'h' } else { '-' }); // Hidden
                    string.push(if attributes & 0x04 != 0 { 's' } else { '-' }); // System
                    string.push(if attributes & 0x20 != 0 { 'a' } else { '-' }); // Archive
                    string.push(if attributes & 0x400 != 0 { 'l' } else { '-' }); // Reparse point
                } else {
                    for permission in _permissions {
                        let mut rights = Vec::new();

                        if permission.read {
                            rights.push("read");
                        }

                        if permission.write {
                            rights.push("write");
                        }

                        if permission.modify {
                            rights.push("modify");
                        }

                        if permission.read_and_execute {
                            rights.push("read and execute");
                        }

                        if permission.list_folder {
                            rights.push("list folder");
                        }

                        if permission.full_control {
                            rights.push("full control");
                        }

                        let access_type = if permission.allow { "allow" } else { "deny" };

                        string.push_str(&format!(
                            "{}: {} [{}] (mask: 0x{:08X}){}\n",
                            permission.identity,
                            access_type,
                            rights.join(", "),
                            permission.access_mask,
                            if permission.inherited {
                                " [inherited]"
                            } else {
                                ""
                            },
                        ));
                    }
                }
            }
        }

        Ok(string)
    }
}
