mod compress;
mod decompress;

pub use compress::*;
pub use decompress::*;

/// The format of the compressor
#[derive(Debug, Clone, Copy)]
pub enum CompressorFormat {
    /// Xz compression
    Xz,

    /// Gzip compression, commonly used with TAR archives.
    Gzip,

    /// Bzip2 compression
    Bzip2,
}
