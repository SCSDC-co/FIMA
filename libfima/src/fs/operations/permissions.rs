use std::{io, os::unix::fs::PermissionsExt, path::Path};

/// Represent the permissions of a file or a directory
///
/// It is an enum because windows and unix handle permissions differently
pub enum Permissions {
    /// Unix type permissions. Example rwxr-xr-x
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
    /// Windows style permissions
    Windows {
        read: bool,
        write: bool,
        read_and_execute: bool,
        modify: bool,
        full_control: bool,
    },
}

impl Permissions {
    /// Gets the permissions as a file as a [`Permissions`]
    pub fn new<P>(path: P) -> Result<Permissions, io::Error>
    where
        P: AsRef<Path>,
    {
        #[cfg(windows)]
        {
            todo!()
        }

        #[cfg(unix)]
        {
            let mode = path.as_ref().metadata()?.permissions().mode();

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
}
