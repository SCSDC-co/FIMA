mod windows;

/// A collection of common operations to do on an item
pub mod operations;

/// A collection of useful utilies mainly created to not duplicate code
pub mod utility;

/// Gets general metadata from items
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::metadata;
///
/// // Get the owner of an item (cross-platform)
/// metadata::owner("foo.txt"); // "Giuliano"
///
/// // Get the group of an item (cross-platform)
/// metadata::group("foo.txt"); // "users"
/// ```
pub mod metadata;
