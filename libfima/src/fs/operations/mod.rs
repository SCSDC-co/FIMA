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

/// Copies files or directories
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::copy;
///
/// // copies foo.txt into foo2.txt
/// copy::copy_file("foo.txt", "foo2.txt")?;
/// // if destination is a directory it will add the file name, so it's going to be copied at foo/foo.txt
/// copy::copy_file("foo.txt", "foo")?;
///
/// // copies bar.toml and baz into foo
/// copy::copy_file_many(["bar.toml", "baz"], "foo")?;
///
/// // copies barbar/baz into foo
/// copy::copy_dir("barbar/baz", "foo")?;
/// ```
pub mod copy;
