use std::fs;
use std::io::Error;
use std::path::Path;

pub use glob;

/// Checks if a directory is empty
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::utility;
///
/// utility::is_dir_empty("foo")?; // true
/// ```
pub fn is_dir_empty<P>(dir: P) -> Result<bool, Error>
where
    P: AsRef<Path>,
{
    Ok(fs::read_dir(dir.as_ref())?.next().is_none())
}
