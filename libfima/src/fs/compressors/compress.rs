use std::{fs::File, io, path::Path};

/// Compress a `path` into `output`
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::compressors;
///
/// compressors::compress_file("foo.txt", "foo.txt.xz", compressors::CompressorFormat::Xz, 9)?;
/// ```
pub fn compress_file<P, K>(
    path: P,
    output: K,
    format: super::CompressorFormat,
    level: u32,
) -> anyhow::Result<()>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let path = crate::fs::utility::expand_path(path);
    let output = crate::fs::utility::expand_path(output);

    if path.is_dir() {
        anyhow::bail!(
            "Can't compress {}, it is a directory, only files are supported.",
            path.display()
        );
    }

    let mut file = File::open(&path)?;
    let mut output_file = File::create_new(&output)?;

    match format {
        super::CompressorFormat::Xz => {
            let mut encoder = xz::write::XzEncoder::new(output_file, level);

            io::copy(&mut file, &mut encoder)?;
            encoder.finish()?;
        }
        super::CompressorFormat::Gzip => {
            let mut encoder =
                flate2::write::GzEncoder::new(output_file, flate2::Compression::new(level));

            io::copy(&mut file, &mut encoder)?;
            encoder.finish()?;
        }
        super::CompressorFormat::Bzip2 => {
            let mut encoder =
                bzip2::write::BzEncoder::new(output_file, bzip2::Compression::new(level));

            io::copy(&mut file, &mut encoder)?;
            encoder.finish()?;
        }
        super::CompressorFormat::Zstd => {
            zstd::stream::copy_encode(&mut file, &mut output_file, level as i32)?;
        }
    }

    Ok(())
}
