use std::{
    fs::File,
    path::{Path, PathBuf},
};

use zip::write::SimpleFileOptions;

mod extract;
mod write;

/// All the types of archives available
#[derive(Debug)]
pub enum ArchiveFormat {
    /// Zip archive format
    Zip,
}

#[derive(Debug)]
enum ArchiveType {
    Zip {
        zip: zip::ZipWriter<File>,
        options: SimpleFileOptions,
    },
}

#[derive(Debug)]
/// A struct representing an archive
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::archive;
///
/// let mut archive = archive::Archive::new("foo.zip", fs::archive::ArchiveFormat::Zip)?;
///
/// // add files:
/// archive.add_file("foo.txt")?;
/// archive.add_file("bar.txt")?;
/// archive.add_file("baz.txt")?;
/// // or
/// archive.add_file_many(&[
///  "foo.txt",
///  "bar.txt",
///  "baz.txt",
/// ])?;
///
/// // add directories:
/// archive.add_directory("foo")?;
/// archive.add_directory("bar")?;
/// archive.add_directory("baz")?;
/// // or
/// archive.add_directory_many(&[
///  "foo",
///  "bar",
///  "baz",
/// ])?;
///
/// // always call `.finish()` when finished with the archive
/// // this will destroy the object
/// archive.finish()?;
/// ```
pub struct Archive {
    /// The format the archive is in
    pub format: ArchiveFormat,

    /// The path of the archive
    pub path: PathBuf,

    archive: ArchiveType,
}

impl Archive {
    /// Creates an archive of type `format` in `path`
    pub fn new<P>(path: P, format: ArchiveFormat) -> anyhow::Result<Archive>
    where
        P: AsRef<Path>,
    {
        let path = path.as_ref();

        let file = File::create(path)?;

        let archive: ArchiveType = match format {
            ArchiveFormat::Zip => ArchiveType::Zip {
                zip: zip::ZipWriter::new(file),
                options: SimpleFileOptions::default(),
            },
        };

        Ok(Archive {
            path: PathBuf::from(path),
            format,
            archive,
        })
    }

    /// Finalize the archive and closes it completely
    pub fn finish(self) -> anyhow::Result<()> {
        match self.archive {
            ArchiveType::Zip { zip, options: _ } => {
                zip.finish()?;
            }
        }

        Ok(())
    }
}
