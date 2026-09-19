#![warn(missing_docs)]

pub mod fs;

/// The library version, taken from `Cargo.toml`.
pub const VERSION: &str = env!("CARGO_PKG_VERSION");
