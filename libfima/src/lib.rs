#![warn(missing_docs)]

//! A cross-platform filesystem toolkit.
//!
//! `libfima` provides utilities for working with filesystems,
//! including file metadata, operations, and other filesystem-related
//! functionality.
//!
//! # Modules
//!
//! - [`fs`] — filesystem-related functionality.

/// A filesystem toolkit
pub mod fs;

/// The library version, taken from `Cargo.toml`.
pub const VERSION: &str = env!("CARGO_PKG_VERSION");
