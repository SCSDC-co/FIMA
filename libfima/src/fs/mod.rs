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

#[cfg(any(feature = "path-extension"))]
pub mod extentions;
