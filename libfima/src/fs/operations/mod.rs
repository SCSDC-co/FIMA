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
