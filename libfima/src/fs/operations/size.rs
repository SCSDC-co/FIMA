use std::{fs, io, path::Path};

/// Get the size in bytes of an item
///
/// It returns the file size if it is a file
/// Or it returns the sum of the sizes of the elements in a directory,
/// without following subdirectories
///
/// It doesn't follow symlinks
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::size;
///
/// size::size("foo.txt")?; // 64
/// size::size("bar")?;     // 128
/// ```
pub fn size<P>(path: P) -> Result<u64, io::Error>
where
    P: AsRef<Path>,
{
    // we won't follow symlinks as we are counting the path size itself
    let metadata = path.as_ref().symlink_metadata()?;

    let mut size: u64 = 0;

    if metadata.is_dir() {
        for item in fs::read_dir(path)? {
            let item_metadata = item?.metadata()?;

            if item_metadata.is_file() {
                size += item_metadata.len();
            }

            // we won't iterate into subdirectories as it's slow on large directories
        }
    } else {
        size = metadata.len();
    }

    Ok(size)
}

/// Get the items count of an item
///
/// If it is a file it returns 1
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::size;
///
/// size::item_count("foo.txt")?; // 1
/// size::item_count("bar")?;     // 30
/// ```
pub fn item_count<P>(path: P) -> Result<u64, io::Error>
where
    P: AsRef<Path>,
{
    let metadata = path.as_ref().symlink_metadata()?;

    let mut count: u64 = 0;

    if metadata.is_dir() {
        for _ in fs::read_dir(path)? {
            count += 1;
        }
    } else {
        count = 1;
    }

    Ok(count)
}
