use std::fs;
use std::io::Error;
use std::path::Path;

pub use glob;

/// Checks if a directory is empty
pub fn is_dir_empty<P>(dir: P) -> Result<bool, Error>
where
    P: AsRef<Path>,
{
    Ok(fs::read_dir(dir.as_ref())?.next().is_none())
}

/// Checks if a directory is an ancestor of another
pub fn is_ancestor<P>(path: P) -> Result<bool, Error>
where
    P: AsRef<Path>,
{
    let path = path.as_ref();

    for parent in std::env::current_dir()?.ancestors() {
        if path == parent {
            return Ok(true);
        }
    }

    Ok(false)
}

/// Checks if the directory is the root
pub fn is_root<P>(path: P) -> Result<bool, std::io::Error>
where
    P: AsRef<Path>,
{
    let path = std::fs::canonicalize(path)?;

    Ok(path.parent().is_none())
}
