mod compress;
mod decompress;

use std::fmt;

pub use compress::*;
pub use decompress::*;

/// The format of the compressor
#[derive(Debug, Clone, Copy, Default, PartialEq)]
pub enum CompressorFormat {
    /// Xz compression
    Xz,

    /// Gzip compression, commonly used with TAR archives (default)
    #[default]
    Gzip,

    /// Bzip2 compression
    Bzip2,

    /// Zstd compression
    Zstd,
}

impl fmt::Display for CompressorFormat {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Xz => write!(f, "XZ"),
            Self::Gzip => write!(f, "GZIP"),
            Self::Bzip2 => write!(f, "BZIP2"),
            Self::Zstd => write!(f, "Zstandard"),
        }
    }
}
