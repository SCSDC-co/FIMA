use std::{io, path::Path};
use time;

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

/// Returns the last modified date of an item formatted
///
/// This is the format: `[day]/[month]/[year] [hour]:[minute]:[second]`
///
/// # Examples
///
/// ```
/// use libfima::fs::metadata;
///
/// metadata::file_time_formatted("foo")?; // 30/11/2026 22:44:43
/// ```
pub fn file_time_formatted<P>(path: P) -> anyhow::Result<String>
where
    P: AsRef<Path>,
{
    let path = path.as_ref();
    let format = time::format_description::parse_owned::<3>(
        "[day]/[month]/[year] [hour]:[minute]:[second]",
    )?;

    let time = file_time(path)?;

    Ok(time::OffsetDateTime::from(time).format(&format)?)
}

/// Returns the last modified date of an item with a custom format
///
/// For the full syntax of the formats: <https://time-rs.github.io/book/api/format-description.html>
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::metadata;
///
/// metadata::file_time_formatted_custom("foo", "[year]/[month]/[day] [hour]:[minute]:[second]")?;
/// ```
pub fn file_time_formatted_custom<P, S>(path: P, format: S) -> anyhow::Result<String>
where
    P: AsRef<Path>,
    S: AsRef<str>,
{
    let path = path.as_ref();
    let format = time::format_description::parse_owned::<3>(&format.as_ref())?;

    let time = file_time(path)?;

    Ok(time::OffsetDateTime::from(time).format(&format)?)
}
