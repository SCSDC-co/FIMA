// keep that first empty line, it is just to not show the content of the docs in the module overview
// of fs
//!
//! The main function of this module is to simplify the usage of libfima by adding custom methods to
//! Path and PathBuf.
//!
//! # Examples
//!
//! ```rust,no_run,ignore
//! use libfima::fs::extensions::PathExt;
//!
//! let path = Path::new("foo.txt");
//!
//! path.create(false)?;
//!
//! path.rename("bar.txt")?;
//!
//! path.size()?;
//! ```

#[cfg(feature = "path-extension")]
mod path_ext;

#[cfg(feature = "path-extension")]
pub use path_ext::*;

#[cfg(feature = "compressor-extension")]
mod compressor_ext;

#[cfg(feature = "compressor-extension")]
pub use compressor_ext::*;
