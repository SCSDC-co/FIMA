use std::{fs::File, io::Write, path::Path};
use walkdir::WalkDir;

fn add_file_to_zip<P, K>(
    zip: &mut zip::ZipWriter<File>,
    path: P,
    root: K,
    options: zip::write::SimpleFileOptions,
) -> anyhow::Result<()>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let path = crate::fs::utility::expand_path(path);
    let mut root = crate::fs::utility::expand_path(root);

    if !root.is_empty() {
        root = root.canonicalize()?;
    }

    if root.file_name().unwrap() == path.components().next().unwrap().as_os_str() {
        root = root.parent().unwrap().to_path_buf();
    }

    let canonical_path = path.canonicalize()?;
    let relative_path = canonical_path.strip_prefix(&root)?;

    zip.start_file(relative_path.to_str().unwrap(), options)?;
    zip.write_all(std::fs::read(relative_path)?.as_slice())?;

    Ok(())
}

impl super::Archive {
    /// Add one file to the archive
    pub fn add_file<P>(&mut self, path: P) -> anyhow::Result<()>
    where
        P: AsRef<Path>,
    {
        let path = crate::fs::utility::expand_path(path);

        match self.archive {
            super::ArchiveType::Zip {
                ref mut writer,
                options,
                ..
            } => {
                let canonical_path = path.canonicalize()?;
                let root = canonical_path.parent().unwrap();

                add_file_to_zip(writer, &path, root, options)?;
            }
        }

        Ok(())
    }

    /// Add multiple files into the archive
    pub fn add_file_many<P, K>(&mut self, paths: K) -> anyhow::Result<()>
    where
        K: IntoIterator<Item = P>,
        P: AsRef<Path>,
    {
        for path in paths {
            self.add_file(path)?;
        }

        Ok(())
    }

    /// Add one directory to the archive
    pub fn add_directory<P>(&mut self, path: P) -> anyhow::Result<()>
    where
        P: AsRef<Path>,
    {
        let path = crate::fs::utility::expand_path(path);
        let root = path.parent().unwrap();

        match self.archive {
            super::ArchiveType::Zip {
                ref mut writer,
                options,
                ..
            } => {
                for item in WalkDir::new(&path) {
                    let mut item = item?.into_path();

                    if path.is_absolute() {
                        item = item.strip_prefix(root)?.to_path_buf();
                    }

                    if item.is_file() {
                        add_file_to_zip(writer, &item, root, options)?;
                    } else if item.is_dir() {
                        writer.add_directory(&*item.to_str().unwrap(), options)?;
                    }
                }
            }
        }

        Ok(())
    }

    /// Add multiple directories into the archive
    pub fn add_directory_many<P, K>(&mut self, paths: K) -> anyhow::Result<()>
    where
        K: IntoIterator<Item = P>,
        P: AsRef<Path>,
    {
        for path in paths {
            self.add_directory(path)?;
        }

        Ok(())
    }
}
