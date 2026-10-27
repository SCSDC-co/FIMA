use std::{fs::File, io, path::Path};

use xz::write::XzEncoder;

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

    match format {
        super::CompressorFormat::Xz => {
            let mut file = File::open(&path)?;
            let output_file = File::create_new(&output)?;
            let mut encoder = XzEncoder::new(output_file, level);

            io::copy(&mut file, &mut encoder)?;
            encoder.finish()?;
        }
    }

    Ok(())
}
