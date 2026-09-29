use std::path::Path;

#[cfg(unix)]
use file_owner::PathExt;

#[cfg(windows)]
use std::{os::windows::io::AsRawHandle, ptr};

#[cfg(windows)]
use windows::{
    Win32::{
        Foundation::{HANDLE, LocalFree},
        Security::{
            GROUP_SECURITY_INFORMATION, GetSecurityInfo, LookupAccountSidW,
            OWNER_SECURITY_INFORMATION, PSECURITY_DESCRIPTOR, PSID, SE_FILE_OBJECT,
        },
    },
    core::PWSTR,
};

#[cfg(windows)]
fn security_info<P>(
    path: P,
    security_information: u32,
) -> Result<(PSID, PSECURITY_DESCRIPTOR), Box<dyn std::error::Error>>
where
    P: AsRef<Path>,
{
    let file = std::fs::File::open(path)?;
    let handle = HANDLE(file.as_raw_handle());

    let mut sid = ptr::null_mut();
    let mut security_descriptor = ptr::null_mut();

    unsafe {
        GetSecurityInfo(
            handle,
            SE_FILE_OBJECT,
            security_information,
            Some(&mut sid),
            None,
            None,
            None,
            Some(&mut security_descriptor),
        )?;
    }

    Ok((sid, security_descriptor))
}

#[cfg(windows)]
fn sid_name(sid: PSID) -> Result<String, Box<dyn std::error::Error>> {
    let mut name_size = 0;
    let mut domain_size = 0;
    let mut sid_type = 0;

    unsafe {
        LookupAccountSidW(
            None,
            sid,
            PWSTR::null(),
            &mut name_size,
            PWSTR::null(),
            &mut domain_size,
            &mut sid_type,
        );
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

    Ok(String::from_utf16_lossy(&name[..name_size as usize]))
}

/// Gets the owner of a file (cross-platform)
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::metadata;
///
/// metadata::owner("foo.txt"); // "Giuliano"
/// ```
pub fn owner<P>(path: P) -> Result<String, Box<dyn std::error::Error>>
where
    P: AsRef<Path>,
{
    let path = path.as_ref();

    let owner: String;

    #[cfg(unix)]
    {
        owner = path.owner()?.name()?.unwrap_or("Unknown".to_string());
    }

    #[cfg(windows)]
    {
        let (sid, security_descriptor) = security_info(path, OWNER_SECURITY_INFORMATION)?;

        let sid_name = sid_name(sid);

        unsafe {
            LocalFree(Some(security_descriptor));
        }

        owner = sid_name;
    }

    Ok(owner)
}

/// Gets the group of a file (cross-platform)
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::metadata;
///
/// metadata::group("foo.txt"); // "users"
/// ```
pub fn group<P>(path: P) -> Result<String, Box<dyn std::error::Error>>
where
    P: AsRef<Path>,
{
    let path = path.as_ref();

    let group: String;

    #[cfg(unix)]
    {
        group = path.group()?.name()?.unwrap_or("Unknown".to_string());
    }

    #[cfg(windows)]
    {
        let (sid, security_descriptor) = security_info(path, GROUP_SECURITY_INFORMATION)?;

        let sid_name = sid_name(sid);

        unsafe {
            LocalFree(Some(security_descriptor));
        }

        group = sid_name;
    }

    Ok(group)
}
