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
    let destination = destination.as_ref();

    for path in paths {
        std::fs::rename(path, destination.join(path.as_ref().file_name().unwrap()))?;
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
    let old_name = old_name.as_ref();
    let new_name = new_name.as_ref();

    std::fs::rename(old_name, new_name)
}
