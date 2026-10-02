/// A module to remove a file or a directory, or many of them
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
/// // Removes a single item
/// remove::remove(
///     "foo.txt",
///     &options,
/// )?;
///
/// // Removes multiple items
/// remove::remove_many(
///     &[
///         "foo.txt",
///         "bar.txt",
///         "baz.txt",
///     ],
///     &options,
/// )?;
///
/// // removes items that match a glob pattern
/// remove::remove_glob("**/*.rs", &options)?;
/// ```
pub mod remove;

/// Get the size of directories and files
///
/// It doesn't follow symlinks and doesn't recurse into subdirectories
///
/// # Example
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::size;
///
/// // Gets the size in bytes
/// size::size("foo.txt")?; // 64
/// size::size("bar")?;     // 128
///
/// // Gets the number of items in a directory
/// size::item_count("foo.txt")?; // 1
/// size::item_count("bar")?;     // 30
/// ```
pub mod size;

pub mod rename;

pub mod create;
