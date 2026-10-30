mod compress;
mod decompress;

pub use compress::*;
pub use decompress::*;

/// The format of the compressor
#[derive(Debug, Clone, Copy, Default)]
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
