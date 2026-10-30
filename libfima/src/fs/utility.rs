// keep that first empty line, it is just to not show the content of the docs in the module overview
// of fs
//!
//! # Examples
//!
//! ```rust,no_run,ignore
//! use libfima::fs::utility;
//!
//! utility::is_dir_empty("foo"); // false
//! utility::is_dir_empty("bar"); // true
//!
//! utility::is_ancestor("foo", "bar"); // false
//! utility::is_ancestor("foo", "foo/bar"); // true
//!
//! utility::is_ancestor_of_current_directory("foo"); // false
//! utility::is_ancestor_of_current_directory("../bar"); // true
//!
//! utility::is_root("foo"); // false
//! utility::is_root("/"); // true
//! utility::is_root("C:\\"); // true
//!
//! utility::expand_path("~/foo/bar"); // /home/user/foo/bar
//! utility::expand_path("foo/bar"); // foo/bar
//! ```

use std::fs;
use std::io;
use std::path::{Path, PathBuf};

pub use glob;

/// Checks if a directory is empty
pub fn is_dir_empty<P>(dir: P) -> io::Result<bool>
where
    P: AsRef<Path>,
{
    Ok(fs::read_dir(dir.as_ref())?.next().is_none())
}

/// Checks if a directory is an ancestor of the current directory
pub fn is_ancestor_of_cwd<P>(path: P) -> io::Result<bool>
where
    P: AsRef<Path>,
{
    let path = expand_path(path).canonicalize()?;

    Ok(std::env::current_dir()?.starts_with(path))
}

/// Checks if a directory is an ancestor of another
pub fn is_ancestor<P, K>(path: P, parent: K) -> io::Result<bool>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let path = expand_path(path).canonicalize()?;
    let parent = expand_path(parent).canonicalize()?;

    Ok(parent.starts_with(path))
}

/// Checks if the directory is the root
pub fn is_root<P>(path: P) -> io::Result<bool>
where
    P: AsRef<Path>,
{
    let path = std::fs::canonicalize(path)?;

    Ok(path.parent().is_none())
}

/// Expand the tilde of a path, if there is one
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
