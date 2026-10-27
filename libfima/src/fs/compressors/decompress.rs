use std::{fs::File, io, path::Path};

use xz::read::XzDecoder;

/// Decompress a `path` into `output`
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::compressors;
///
/// compressors::compress_file("foo.txt.xz", "foo.txt", compressors::CompressorFormat::Xz)?;
/// ```
pub fn decompress_file<P, K>(
    path: P,
    output: K,
    format: super::CompressorFormat,
) -> anyhow::Result<()>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let path = crate::fs::utility::expand_path(path);
    let output = crate::fs::utility::expand_path(output);

    match format {
        super::CompressorFormat::Xz => {
            let file = File::open(&path)?;
            let mut decoder = XzDecoder::new(file);

            let mut new_file = File::create_new(output)?;

            io::copy(&mut decoder, &mut new_file)?;
        }
    }

    Ok(())
}
