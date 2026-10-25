use std::{
    fs::File,
    io,
    path::{Path, PathBuf},
};

/// List items inside an archive
///
/// The reason why this isn't a struct method is explained here: [`super::extract()`]
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::{fs, fs::archive};
///
/// let archive_file = "foo.zip";
/// let output_path = "output";
///
/// let mut archive = archive::Archive::new(archive_file, fs::archive::ArchiveFormat::Zip)?;
///
/// archive.add_file_many(["foo.txt", "bar.json"])?;
/// archive.add_directory("baz/barbar")?;
///
/// // we need to call `.clone()` because `archive` will be
/// // destroyed after calling `.finish()`
/// let format = archive.format.clone();
///
/// archive.finish()?;
///
/// for files in archive::list(zip_file, format)? {
///     println!("{}", files.display())
/// }
/// ```
pub fn list<P>(archive: P, archive_format: super::ArchiveFormat) -> anyhow::Result<Vec<PathBuf>>
where
    P: AsRef<Path>,
{
    let path = crate::fs::utility::expand_path(archive);

    let mut paths: Vec<PathBuf> = vec![];

    match archive_format {
        super::ArchiveFormat::Zip => {
            let archive_file = File::open(path)?;

            let mut archive = zip::ZipArchive::new(archive_file)?;

            for i in 0..archive.len() {
                let item = archive.by_index(i)?;

                paths.push(item.enclosed_name().ok_or(io::Error::new(
                    io::ErrorKind::InvalidFilename,
                    "Error: invalid path",
                ))?);
            }
        }
    }

    Ok(paths)
}
