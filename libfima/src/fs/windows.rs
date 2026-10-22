#[cfg(windows)]
use std::{
    os::windows::{fs::OpenOptionsExt, io::AsRowHandle},
    path::Path,
    ptr,
};

#[cfg(windows)]
use windows::{
    Win32::{
        Foundation::{HANDLE, HLOCAL, LocalFree},
        Security::{
            ACL, DACL_SECURITY_INFORMATION, GROUP_SECURITY_INFORMATION, GetSecurityInfo,
            LookupAccountSidW, OWNER_SECURITY_INFORMATION, PSECURITY_DESCRIPTOR, PSID,
            SE_FILE_OBJECT, SID_NAME_USE,
        },
    },
    core::PWSTR,
};

/// Owns a Windows security descriptor and frees it on drop
#[cfg(windows)]
pub(crate) struct SecurityDescriptorGuard(pub(crate) PSECURITY_DESCRIPTOR);

#[cfg(windows)]
impl Drop for SecurityDescriptorGuard {
    fn drop(&mut self) {
        if !self.0.is_null() {
            unsafe {
                let_ = LocalFree(Some(HLOCAL(self.0)));
            }
        }
    }
}

/// Retrieves the owner, group and DACL of a filesystem object
#[cfg(windows)]
pub(crate) fn security_info<P>(
    path: P,
    security_information: u32,
) -> anyhow::Result<(PSID, PSID, *mut ACL, PSECURITY_DESCRIPTOR)>
where
    P: AsRef<Path>,
{
    const FILE_FLAG_BACKUP_SEMANTICS: u32 = 0x0200_0000;

    let file = std::fs::OpenOptions::new()
        .read(true)
        .custom_flags(FILE_FLAG_BACKUP_SEMANTICS)
        .open(path)?;

    let handle = HANDLE(file.as_row_handle());

    let mut owner = ptr::null_mut();
    let mut group = ptr::null_mut();
    let mut dacl = ptr::null_mut();
    let mut security_descriptor = ptr::null_mut();

    let requested_information = security_information
        | OWNER_SECURITY_INFORMATION
        | GROUP_SECURITY_INFORMATION
        | DACL_SECURITY_INFORMATION;

    unsafe {
        GetSecurityInfo(
            handle,
            SE_FILE_OBJECT,
            requested_information,
            Some(&mut owner),
            Some(&mut group),
            Some(&mut dacl),
            None,
            Some(&mut security_descriptor),
        )
    }

    Ok((owner, group, dacl, security_descriptor))
}

/// Resolves a Windows SID to an account name
#[cfg(windows)]
pub(crate) fn sid_name(sid: PSID) -> anyhow::Result<String> {
    let mut name_size = 0;
    let mut domain_size = 0;
    let mut sid_type = SID_NAME_USE(0);

    // The first call retrieves the required buffer sizes
    unsafe {
        let _ = LookupAccountSidW(
            None,
            sid,
            PWSTR::null(),
            &mut name_size,
            PWSTR::null(),
            &mut domain_size,
            &mut sid_type,
        );
    }

    if name_size == 0 {
        anyhow::bail!("Unable to resolve Windows SID");
    }

    let mut name = vec![0u16; name_size as usize];
    let mut domain = vec![0u16; domain_size as usize];

    unsafe {
        LookupAccountSidW(
            None,
            sid,
            PWSTR(name.as_mut_ptr()),
            &mut name_size,
            PWSTR(domain.as_mut_ptr()),
            &mut domain_size,
            &mut sid_type,
        )?;
    }

    let name = String::from_utf16_lossy(&name[..name_size.saturating_sub(1) as usize]);

    if domain_size > 1 {
        let domain = String::from_utf16_lossy(&domain[..domain_size.saturating_sub(1) as usize]);

        Ok(format!("{domain}\\{name}"))
    } else {
        Ok(name)
    }
}
