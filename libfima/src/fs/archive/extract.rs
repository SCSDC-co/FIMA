use std::fs;
use std::fs::File;
use std::io;
use std::path::{Path, PathBuf};

/// Extract an archive into output
///
/// It's not on the [`super::Archive`] struct because a ZIP archive for being extracted needs to be
/// finished with `Archive::finish()`, but it will destroy the object so you won't be able to call
/// it again.
///
/// It returns a [`Vec<std::io::Error>`] because the function still needs to continue to other files
/// when failing to extract one.
///
/// # Arguments
///
/// - `archive` - The archive to extract
/// - `output` - The output directory
/// - `archive_format` - The type of the archive
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
/// archive.add_file_many(["foo.txt", "bar.json"], |path| {})?;
/// archive.add_directory("baz/barbar", |path| {})?;
///
/// // we need to call `.clone()` because `archive` will be
/// // destroyed after calling `.finish()`
/// let format = archive.format.clone();
///
/// archive.finish()?;
///
/// let errs = archive::extract(archive_file, output_path, format, |path| {
///     println!("extracting {}", path.display())
/// })?;
///
/// for err in errs {
///     eprintln!("{}", err);
/// }
/// ```
pub fn extract<P, K, F>(
    archive: P,
    output: K,
    archive_format: super::ArchiveFormat,
    mut action: F,
) -> anyhow::Result<Vec<io::Error>>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
    F: FnMut(PathBuf),
{
    let path = crate::fs::utility::expand_path(output);

    fs::create_dir_all(&path)?;

    let archive_file = File::open(&archive)?;

    let mut errors: Vec<io::Error> = vec![];

    match archive_format {
        super::ArchiveFormat::Zip => {
            let mut archive = zip::ZipArchive::new(archive_file)?;

            for i in 0..archive.len() {
                let mut item = match archive.by_index(i) {
                    Ok(i) => i,
                    Err(e) => {
                        errors.push(e.into());
                        continue;
                    }
                };

                let out_path = match item.enclosed_name() {
                    Some(e) => path.join(e),
                    None => {
                        errors.push(io::Error::new(
                            io::ErrorKind::InvalidFilename,
                            format!(
                                "Unable to extract file {:?} because it has an invalid path.",
                                item.name()
                            ),
                        ));
                        continue;
                    }
                };

                if item.is_dir() {
                    match fs::create_dir_all(&out_path) {
                        Err(e) => {
                            errors.push(e);
                            continue;
                        }
                        Ok(_) => {}
                    };
                } else {
                    match out_path.parent() {
                        Some(p) => {
                            if !p.exists() {
                                match fs::create_dir_all(p) {
                                    Err(e) => {
                                        errors.push(e);
                                        continue;
                                    }
                                    Ok(_) => {}
                                };
                            }
                        }
                        None => {}
                    }

                    match File::create(&out_path)
                        .and_then(|mut outfile| io::copy(&mut item, &mut outfile))
                    {
                        Ok(_) => {}
                        Err(e) => {
                            errors.push(e);
                            continue;
                        }
                    }
                }

                action(out_path);
            }
        }
        super::ArchiveFormat::Tar => {
            let mut archive = tar::Archive::new(archive_file);

            for item in archive.entries()? {
                let mut item = match item {
                    Ok(f) => f,
                    Err(e) => {
                        errors.push(e);
                        continue;
                    }
                };

                let item_path = item.path().unwrap().to_path_buf();

                if item.header().entry_type().is_dir() {
                    crate::fs::operations::create::create_dir_all(path.join(&item_path))?;
                } else {
                    let dest = path.join(&item_path);

                    crate::fs::operations::create::create_dir_all(dest.parent().unwrap())?;

                    match File::create(dest)
                        .and_then(|mut outfile| io::copy(&mut item, &mut outfile))
                    {
                        Ok(_) => {}
                        Err(e) => {
                            errors.push(e);
                            continue;
                        }
                    }
                }

                action(item_path);
            }
        }
    }

    Ok(errors)
}
