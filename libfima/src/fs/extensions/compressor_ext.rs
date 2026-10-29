use std::path::Path;

use crate::fs::compressors;

/// Extension trait providing convenient functions for compressing and decompressing files
pub trait CompressorExt {
    /// Compress the file into output
    ///
    /// Wrapper around `compressors::compress_file()`
    fn compress<P>(
        &self,
        output: P,
        format: compressors::CompressorFormat,
        level: u32,
    ) -> anyhow::Result<()>
    where
        P: AsRef<Path>;

    /// Decompress the file into output
    ///
    /// Wrapper around `compressors::decompress_file()`
    fn decompress<P>(&self, output: P, format: compressors::CompressorFormat) -> anyhow::Result<()>
    where
        P: AsRef<Path>;
}

impl CompressorExt for Path {
    fn compress<P>(
        &self,
        output: P,
        format: compressors::CompressorFormat,
        level: u32,
    ) -> anyhow::Result<()>
    where
        P: AsRef<Path>,
    {
        compressors::compress_file(self, output, format, level)
    }

    fn decompress<P>(&self, output: P, format: compressors::CompressorFormat) -> anyhow::Result<()>
    where
        P: AsRef<Path>,
    {
        compressors::decompress_file(self, output, format)
    }
}
