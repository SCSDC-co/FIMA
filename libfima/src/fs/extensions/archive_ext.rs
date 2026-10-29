use std::{
    io,
    path::{Path, PathBuf},
};

use crate::fs::archive;

/// Extension trait providing convenient functions for archiving paths
pub trait ArchiveExt {
    /// Puts the contents into an archive
    ///
    /// It creates the archive with `archive::Archive::new()` and then adds the content of the path
    /// into it
    fn create_archive<P, F>(
        &self,
        output: P,
        format: archive::ArchiveFormat,
        action: F,
    ) -> anyhow::Result<archive::Archive>
    where
        P: AsRef<Path>,
        F: FnMut(PathBuf);

    /// List the archive
    ///
    /// Wrapper around `archive::list()`
    fn list_archive(&self, format: archive::ArchiveFormat) -> anyhow::Result<Vec<PathBuf>>;

    /// Extract the archive
    ///
    /// Wrapper around `archive::extract()`
    fn extract_archive<P, F>(
        &self,
        output: P,
        format: archive::ArchiveFormat,
        action: F,
    ) -> anyhow::Result<Vec<io::Error>>
    where
        P: AsRef<Path>,
        F: FnMut(PathBuf);
}

impl ArchiveExt for Path {
    fn create_archive<P, F>(
        &self,
        output: P,
        format: archive::ArchiveFormat,
        action: F,
    ) -> anyhow::Result<archive::Archive>
    where
        P: AsRef<Path>,
        F: FnMut(PathBuf),
    {
        let mut archive = archive::Archive::new(output, format)?;

        if self.is_dir() {
            archive.add_directory(self, action)?;
        } else {
            archive.add_file(self, action)?;
        }

        Ok(archive)
    }

    fn list_archive(&self, format: archive::ArchiveFormat) -> anyhow::Result<Vec<PathBuf>> {
        archive::list(self, format)
    }

    fn extract_archive<P, F>(
        &self,
        output: P,
        format: archive::ArchiveFormat,
        action: F,
    ) -> anyhow::Result<Vec<io::Error>>
    where
        P: AsRef<Path>,
        F: FnMut(PathBuf),
    {
        archive::extract(self, output, format, action)
    }
}
