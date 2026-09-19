use glob::glob;
use std::fs;
use std::io::{Error, ErrorKind};
use std::path::Path;

/// Options for the remove functions
///
/// # Examples
///
/// ```rust,no_run
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// remove::remove(
///     &"foo.txt".to_string(),
///     &options,
/// )?;
/// ```
#[derive(Debug)]
pub struct Options {
    /// Deletes the directory and all its contents recursively
    pub recursive: bool,
}

/// Removes a file or a directory
///
/// It will panic with a `std::io::Error` if:
///
/// - The path doesn't exist or
/// - The path is a directory, it's not empty and the recursive options is off
///
/// # Examples
///
/// ```rust,no_run
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// remove::remove(
///     &"fest.txt".to_string(),
///     &options,
/// )?;
/// ```
pub fn remove<P>(path: &P, opts: &Options) -> Result<(), Error>
where
    P: AsRef<Path>,
{
    let path = path.as_ref();

    if !path.exists() {
        return Err(Error::new(
            ErrorKind::NotFound,
            format!("The path {} doesn't exist.", path.display()),
        ));
    }

    if path.is_dir() {
        if crate::fs::utility::is_dir_empty(path)? {
            fs::remove_dir(path)?;
        } else {
            if opts.recursive {
                fs::remove_dir_all(path)?;
            } else {
                return Err(Error::new(
                    ErrorKind::DirectoryNotEmpty,
                    format!("The directory {} is not empty.", path.display()),
                ));
            }
        }
    } else {
        fs::remove_file(path)?;
    }

    Ok(())
}

/// Removes multiple files and/or directories.
///
/// It's just a wrapper around `remove`.
///
/// # Examples
///
/// ```rust,no_run
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// remove::remove_many(
///     &[
///         &"foo.txt".to_string(),
///         &"bar.txt".to_string(),
///         &"baz.txt".to_string(),
///     ],
///     &options,
/// )?;
/// ```
pub fn remove_many<P>(paths: &[P], opts: &Options) -> Result<(), Error>
where
    P: AsRef<Path>,
{
    for path in paths {
        remove(path, opts)?;
    }

    Ok(())
}

/// Removes files and/or directories that match a glob pattern.
///
/// It's just a wrapper around `remove`.
///
/// # Examples
///
/// ```rust,no_run
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// // This removes all the rust source files
/// remove::remove_glob("**/*.rs", &options)?;
/// ```
pub fn remove_glob(pattern: &str, opts: &Options) -> Result<(), Error> {
    for entry in glob(pattern).expect("Failed to read glob pattern") {
        remove(&entry?, opts)?;
    }

    Ok(())
}
