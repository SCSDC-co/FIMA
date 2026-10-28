use std::{fs::File, io, path::Path};

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

    let mut file = File::open(&path)?;
    let mut new_file = File::create_new(output)?;

    match format {
        super::CompressorFormat::Xz => {
            let mut decoder = xz::read::XzDecoder::new(file);

            io::copy(&mut decoder, &mut new_file)?;
        }
        super::CompressorFormat::Gzip => {
            let mut decoder = flate2::read::GzDecoder::new(file);

            io::copy(&mut decoder, &mut new_file)?;
        }
        super::CompressorFormat::Bzip2 => {
            let mut decoder = bzip2::read::BzDecoder::new(file);

            io::copy(&mut decoder, &mut new_file)?;
        }
        super::CompressorFormat::Zstd => {
            zstd::stream::copy_decode(&mut file, &mut new_file)?;
        }
    }

    Ok(())
}
