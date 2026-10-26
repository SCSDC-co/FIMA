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

fn add_file_to_zip(
    zip: &mut zip::ZipWriter<File>,
    path: PathBuf,
    options: zip::write::SimpleFileOptions,
) -> anyhow::Result<()> {
    zip.start_file(path.to_str().unwrap(), options)?;
    zip.write_all(std::fs::read(path)?.as_slice())?;

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

        match self.archive {
            super::ArchiveType::Zip {
                ref mut writer,
                options,
                ..
            } => {
                let canonical_path = path.canonicalize()?;
                let root = canonical_path.parent().unwrap();

                let sanitized_path = sanitize_path(&canonical_path, root)?;

                add_file_to_zip(writer, sanitized_path.clone(), options)?;

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
                    let mut item = item?.into_path();

                    if path.is_absolute() {
                        item = item.strip_prefix(root)?.to_path_buf();
                    }

                    let sanitized_path = sanitize_path(&item, root)?;

                    if item.is_file() {
                        add_file_to_zip(writer, sanitized_path.clone(), options)?;
                    } else if item.is_dir() {
                        writer.add_directory(&*item.to_str().unwrap(), options)?;
                    }

                    action(sanitized_path);
                }
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
