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

/// Rename and move items
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::rename;
///
/// // moves multiple items into a directory
/// rename::move_(&["foo/bar.txt", "foo/baz"], "barbar")?;
///
/// // rename an item
/// rename::rename("foo", "bar")?;
/// ```
pub mod rename;

/// Creates an item or more
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::create;
///
/// // Creates a single file
/// create::create_file("foo.txt")?;
///
/// // Creates multiple files
/// create::create_file_many(&[
///     "foo.txt",
///     "bar.txt",
///     "baz.txt",
/// ])?;
///
/// // Creates a single directory
/// create::create_dir("foo")?;
///
/// // Creates multiple directories
/// create::create_dir_many(&[
///     "foo",
///     "bar",
///     "baz",
/// ])?;
///
/// // Creates a directory and its parents
/// create::create_dir_all("foo/bar/baz")?;
///
/// // Creates multiple directories and their parents
/// create::create_dir_all_many(&[
///     "foo/bar",
///     "baz/qux",
///     "quux/corge",
/// ])?;
/// ```
pub mod create;
