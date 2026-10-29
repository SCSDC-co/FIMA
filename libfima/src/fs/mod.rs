//!
//! Useful filesystem toolbox
//!
//! - [`fs::operations`] — Common file operations
//! - [`fs::metadata`] — Get general and specific metadata about an item
//! - [`fs::utility`] — Useful general-purpose utilities
//! - [`fs::archive`] — Working with archives (compressed and not)
//! - [`fs::compressors`] — Working with compressors, like gzip and xz

/// A module for windows-specific functions
mod windows;

/// A collection of common operations to do on an item
pub mod operations;

/// A collection of useful utilities mainly created to not duplicate code
pub mod utility;

/// A module for working with archives
pub mod archive;

/// A module for working with compressors for single files (xz, gzip, bzip2, zstd)
pub mod compressors;

/// Work with general metadata of an item
pub mod metadata;

#[cfg(any(
    feature = "path-extension",
    feature = "compressor-extension",
    feature = "archive-extension"
))]
/// Extensions of Path
pub mod extensions;
