use std::path::Path;

/// Move a list of items into the destination
///
/// If the path is a file the location will be: `destination / file name`
///
/// # Examples:
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::rename;
///
/// rename::move_(&["foo/bar.txt", "foo/baz"], "barbar")?;
/// ```
pub fn move_<P, K>(paths: &[P], destination: K) -> Result<(), std::io::Error>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let destination = crate::fs::utility::expand_path(destination);

    for path in paths {
        let path = crate::fs::utility::expand_path(path);

        std::fs::rename(&path, destination.join(path.file_name().unwrap()))?;
    }

    Ok(())
}

/// Renames an item
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::rename;
///
/// rename::rename("foo", "bar")?;
/// ```
pub fn rename<P, K>(old_name: P, new_name: K) -> Result<(), std::io::Error>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let old_name = crate::fs::utility::expand_path(old_name);
    let new_name = crate::fs::utility::expand_path(new_name);

    std::fs::rename(old_name, new_name)
}
