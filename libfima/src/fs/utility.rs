use std::fs;
use std::io::Error;
use std::path::{Path, PathBuf};

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

/// Expand the tilde of a path, if it exist
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::utility;
///
/// utility::expand_path("~/foo/bar"); // /home/user/foo/bar
/// utility::expand_path("foo/bar"); // foo/bar
/// ```
pub fn expand_path<P>(path: P) -> PathBuf
where
    P: AsRef<Path>,
{
    let path = path.as_ref();

    if path.starts_with("~") {
        let mut path_components = path.components();

        let tilde = path_components
            .next()
            .unwrap()
            .as_os_str()
            .to_str()
            .unwrap();

        let rest_of_the_path: PathBuf = path_components.collect();

        Path::new(&shellexpand::tilde(tilde).to_string()).join(rest_of_the_path)
    } else {
        path.to_path_buf()
    }
}
