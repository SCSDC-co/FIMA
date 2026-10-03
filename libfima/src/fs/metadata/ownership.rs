use std::path::Path;

#[cfg(unix)]
use file_owner::PathExt;

#[cfg(windows)]
use crate::fs::windows;

#[cfg(windows)]
use windows::Win32::Security::{GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION};

/// Gets the owner of a file (cross-platform)
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::metadata;
///
/// metadata::owner("foo.txt"); // "Giuliano"
/// ```
pub fn owner<P>(path: P) -> anyhow::Result<String>
where
    P: AsRef<Path>,
{
    let path = crate::fs::utility::expand_path(path);

    let owner: String;

    #[cfg(unix)]
    {
        owner = path.owner()?.name()?.unwrap_or("Unknown".to_string());
    }

    #[cfg(windows)]
    {
        let (sid, security_descriptor) = windows::security_info(path, OWNER_SECURITY_INFORMATION)?;

        let sid_name = windows::sid_name(sid);

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
/// use libfima::fs::metadata;
///
/// metadata::group("foo.txt"); // "users"
/// ```
pub fn group<P>(path: P) -> anyhow::Result<String>
where
    P: AsRef<Path>,
{
    let path = crate::fs::utility::expand_path(path);

    let group: String;

    #[cfg(unix)]
    {
        group = path.group()?.name()?.unwrap_or("Unknown".to_string());
    }

    #[cfg(windows)]
    {
        let (sid, security_descriptor) = windows::security_info(path, GROUP_SECURITY_INFORMATION)?;

        let sid_name = windows::sid_name(sid);

        unsafe {
            LocalFree(Some(security_descriptor));
        }

        group = sid_name;
    }

    Ok(group)
}
