mod compress;
mod decompress;

pub use compress::*;
pub use decompress::*;

/// The format of the compressor
#[derive(Debug, Clone, Copy)]
pub enum CompressorFormat {
    /// Xz algorithm
    Xz,
}
