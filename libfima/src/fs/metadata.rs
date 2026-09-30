use std::{io, path::Path};
use time;

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
        let (sid, security_descriptor) = windows::security_info(path, GROUP_SECURITY_INFORMATION)?;

        let sid_name = windows::sid_name(sid);

        unsafe {
            LocalFree(Some(security_descriptor));
        }

        group = sid_name;
    }

    Ok(group)
}

/// Returns the last modified date of a file or directory as a `std::time::SystemTime`
///
/// It doesn't follow symlinks as we are getting the last modified date directly of the file and not
/// the file that it points to
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::metadata;
///
/// metadata::file_time("foo")?;
///
/// // Possible output:
/// // SystemTime {
/// //     tv_sec: 1790777179,
/// //     tv_nsec: 304247090,
/// // }
/// ```
pub fn file_time<P>(path: P) -> Result<std::time::SystemTime, io::Error>
where
    P: AsRef<Path>,
{
    let path = path.as_ref();

    Ok(path.symlink_metadata()?.modified()?)
}

/// Returns the last modified date formatted
///
/// This is the default format: [year]/[month]/[day] [hour]:[minute]:[second]
///
/// # Examples
///
/// ```
/// use libfima::fs::metadata;
///
/// metadata::file_time_formatted("foo")?; // 2026/11/30 22:44:43
/// ```
pub fn file_time_formatted<P>(
    path: P,
    format: Option<String>,
) -> Result<String, Box<dyn std::error::Error>>
where
    P: AsRef<Path>,
{
    let path = path.as_ref();
    let format = time::format_description::parse_owned::<3>(
        &format.unwrap_or("[year]/[month]/[day] [hour]:[minute]:[second]".to_string()),
    )?;

    let time = file_time(path)?;

    Ok(time::OffsetDateTime::from(time).format(&format)?)
}
