#![warn(missing_docs)]

//! A cross-platform filesystem toolkit.
//!
//! `libfima` provides utilities for working with filesystems,
//! including file metadata, operations, and other filesystem-related
//! functionality.
//!
//! # Main Features
//!
//! - General file system operations ([`fs::operations`])
//! - Useful utilities ([`fs::utility`])
//! - Archive and compressors support ([`fs::archive`], [`fs::compressors`])
//! - General metadata ([`fs::metadata`])
//! - Multi-platform
//! - Minimal but customizable as you like
//!
//! # Modules
//!
//! - [`fs`] — filesystem-related functionality.
//!
//! # Crate Features
//!
//! - `path-extension` — adds the `PathExt` trait into Path and PathBuf

/// A filesystem toolkit
pub mod fs;

/// The library version, taken from `Cargo.toml`.
pub const VERSION: &str = env!("CARGO_PKG_VERSION");
