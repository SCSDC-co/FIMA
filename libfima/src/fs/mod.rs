/// A module for windows-specific functions
mod windows;

/// A collection of common operations to do on an item
pub mod operations;

/// A collection of useful utilities mainly created to not duplicate code
pub mod utility;

/// Get general metadata from items
///
/// It implements a wrapper around `magic` that exposes useful methods
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
///
/// // Get the last modified time:
/// metadata::file_time("foo")?;
/// // SystemTime {
/// //     tv_sec: 1790777179,
/// //     tv_nsec: 304247090,
/// // }
/// metadata::file_time_formatted("foo")?; // 30/11/2026 22:44:43
/// metadata::file_time_formatted_custom("foo", "[year]/[month]/[day] [hour]:[minute]:[second]")?;
/// // 2026/11/30 22:44:43
/// ```
///
/// ---
///
/// [`metadata::Magic`]:
///
/// ```rust,no_run,ignore
/// use libgima::fs::metadata;
///
/// // The variable must be `mut` as the methods will change the flags of it
/// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
///
/// let path = "foo.txt";
///
/// let mime = magic.mime_type(path)?; // "text/plain"
/// let encoding = magic.encoding(path)?; // "us-ascii"
///
/// // `Magic::file()` uses the current flags (`Magic::flags()`)
/// magic.set_flags(metadata::magic_flags::Flags::MIME)?;
///
/// let file = magic.file("foo.txt")?; // "text/plain; charset=us-ascii"
/// ```
pub mod metadata;
