use std::{
    fs::File,
    io::Write,
    path::{Path, PathBuf},
};
use walkdir::WalkDir;

fn sanitize_path<P, K>(path: P, root: K) -> anyhow::Result<PathBuf>
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

    Ok(relative_path.to_path_buf())
}

fn add_file_to_zip<P, K>(
    zip: &mut zip::ZipWriter<File>,
    archive_path: P,
    source: K,
    options: zip::write::SimpleFileOptions,
) -> anyhow::Result<()>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    zip.start_file(archive_path.as_ref().to_str().unwrap(), options)?;
    zip.write_all(std::fs::read(source)?.as_slice())?;

    Ok(())
}

impl super::Archive {
    /// Add one file to the archive
    pub fn add_file<P, F>(&mut self, path: P, mut action: F) -> anyhow::Result<()>
    where
        P: AsRef<Path>,
        F: FnMut(PathBuf),
    {
        let path = crate::fs::utility::expand_path(path);

        let canonical_path = path.canonicalize()?;
        let root = canonical_path.parent().unwrap();

        let sanitized_path = sanitize_path(&canonical_path, root)?;

        match self.archive {
            super::ArchiveType::Zip {
                ref mut writer,
                options,
                ..
            } => {
                add_file_to_zip(writer, &sanitized_path, canonical_path, options)?;

                action(sanitized_path);
            }
            super::ArchiveType::Tar { ref mut builder } => {
                builder.append_file(&sanitized_path, &mut File::open(sanitized_path.clone())?)?;

                action(sanitized_path);
            }
        }

        Ok(())
    }

    /// Add multiple files into the archive
    pub fn add_file_many<P, K, F>(&mut self, paths: K, mut action: F) -> anyhow::Result<()>
    where
        K: IntoIterator<Item = P>,
        P: AsRef<Path>,
        F: FnMut(PathBuf),
    {
        for path in paths {
            self.add_file(path, &mut action)?;
        }

        Ok(())
    }

    /// Add one directory to the archive
    pub fn add_directory<P, F>(&mut self, path: P, mut action: F) -> anyhow::Result<()>
    where
        P: AsRef<Path>,
        F: FnMut(PathBuf),
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
                    let item = item?.into_path();
                    let archive_path = item.strip_prefix(root)?.to_path_buf();

                    if item.is_file() {
                        add_file_to_zip(writer, &archive_path, item, options)?;
                    } else if item.is_dir() {
                        writer.add_directory(&*archive_path.to_str().unwrap(), options)?;
                    }

                    action(archive_path);
                }
            }
            super::ArchiveType::Tar { ref mut builder } => {
                let sanitized_path = sanitize_path(&path, root)?;

                builder.append_dir_all(&sanitized_path, &path)?;

                action(path);
            }
        }

        Ok(())
    }

    /// Add multiple directories into the archive
    pub fn add_directory_many<P, K, F>(&mut self, paths: K, mut action: F) -> anyhow::Result<()>
    where
        K: IntoIterator<Item = P>,
        P: AsRef<Path>,
        F: FnMut(PathBuf),
    {
        for path in paths {
            self.add_directory(path, &mut action)?;
        }

        Ok(())
    }
}
