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
    let path = crate::fs::utility::expand_path(path);

    // we won't follow symlinks as we are counting the path size itself
    let metadata = path.symlink_metadata()?;

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
    let path = crate::fs::utility::expand_path(path);

    let metadata = path.symlink_metadata()?;

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

/// Converts a size in bytes to a human-readable representation.
///
/// # Arguments
///
/// - `size` - The size in bytes.
/// - `si_units` - Whether to use SI units (`KB`, `MB`, etc.), which divide the size by 1000 instead of 1024.
///
/// # Returns
///
/// A tuple containing the formatted size and its unit as `String`
/// (for example, `("10.85", "MB")`).
///
/// # Examples
///
/// ```
/// use libfima::fs::operations::size;
///
/// assert_eq!(
///     size::make_size_readable(1_500, true),
///     ("1.5".to_string(), "KB".to_string())
/// );
/// assert_eq!(
///     size::make_size_readable(1_000_000, true),
///     ("1".to_string(), "MB".to_string())
/// );
///
/// assert_eq!(
///     size::make_size_readable(1_536, false),
///     ("1.5".to_string(), "KiB".to_string())
/// );
/// assert_eq!(
///     size::make_size_readable(1_048_576, false),
///     ("1".to_string(), "MiB".to_string())
/// );
/// ```
pub fn make_size_readable(size: u64, si_units: bool) -> (String, String) {
    let mut extensions = ["B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"];
    let mut unit = 1024.0;

    if si_units {
        extensions = ["B", "KB", "MB", "GB", "TB", "PB", "EB"];
        unit = 1000.0;
    }

    let mut bytes = size as f64;
    let mut i = 0;

    while bytes >= unit && i < extensions.len() - 1 {
        bytes /= unit;

        i += 1;
    }

    (
        format!("{bytes}").trim_end_matches(".0").to_string(),
        extensions[i].to_string(),
    )
}
