use glob::glob;
use std::{fs, io, path::Path};

/// Options for the remove functions
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// remove::remove(
///     "foo.txt",
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
/// It will return a `std::io::Error` if:
///
/// - The path doesn't exist or
/// - The path is a directory and it's not empty and the recursive options is off
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// remove::remove(
///     "foo.txt",
///     &options,
/// )?;
/// ```
pub fn remove<P>(path: P, opts: &Options) -> Result<(), io::Error>
where
    P: AsRef<Path>,
{
    let path = &crate::fs::utility::expand_path(path);

    if crate::fs::utility::is_root(path)? {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            "You can't delete the root.",
        ));
    }

    if crate::fs::utility::is_ancestor(path)? {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            "You can't delete a parent of the current directory.",
        ));
    }

    if path.is_dir() {
        if crate::fs::utility::is_dir_empty(path)? {
            fs::remove_dir(path)?;
        } else {
            if !opts.recursive {
                return Err(io::Error::new(
                    io::ErrorKind::DirectoryNotEmpty,
                    format!("The directory {} is not empty.", path.display()),
                ));
            }

            fs::remove_dir_all(path)?;
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
/// ```rust,no_run,ignore
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// remove::remove_many(
///     &[
///         "foo.txt",
///         "bar.txt",
///         "baz.txt",
///     ],
///     &options,
/// )?;
/// ```
pub fn remove_many<P>(paths: &[P], opts: &Options) -> Result<(), io::Error>
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
/// ```rust,no_run,ignore
/// use libfima::fs::operations::remove;
///
/// let options = remove::Options {
///     recursive: true
/// };
///
/// // This removes all the rust source files
/// remove::remove_glob("**/*.rs", &options)?;
/// ```
pub fn remove_glob<S>(pattern: S, opts: &Options) -> Result<(), io::Error>
where
    S: AsRef<str>,
{
    let pattern = pattern.as_ref();

    for entry in glob(pattern).expect("Failed to read glob pattern") {
        remove(&entry?, opts)?;
    }

    Ok(())
}
