// keep that first empty line, it is just to not show the content of the docs in the module overview
// of fs
//!
//! It contains a struct and 2 functions for working with archives
//!
//! Supported formats:
//!
//! - `zip` (compressed)
//! - `tar` (not compressed)
//!
//! # Examples
//!
//! ```rust,no_run,ignore
//! use libfima::fs::archive;
//!
//! let zip_file = "foo.zip";
//!
//! let mut archive = archive::Archive::new(zip_file, fs::archive::ArchiveFormat::Zip)?;
//!
//! // add files:
//! archive.add_file("foo.txt", |path| { println!("adding: {}", path.display()); )?;
//! archive.add_file("bar.txt", |path| { println!("adding: {}", path.display()); )?;
//! archive.add_file("baz.txt", |path| { println!("adding: {}", path.display()); )?;
//! // or
//! archive.add_file_many([
//!  "foo.txt",
//!  "bar.txt",
//!  "baz.txt",
//! ], |path| { println!("adding: {}", path.display()); })?;
//!
//! // add directories:
//! archive.add_directory("foo", |path| { println!("adding: {}", path.display()); })?;
//! archive.add_directory("bar", |path| { println!("adding: {}", path.display()); })?;
//! archive.add_directory("baz", |path| { println!("adding: {}", path.display()); })?;
//! // or
//! archive.add_directory_many([
//!  "foo",
//!  "bar",
//!  "baz",
//! ], |path| { println!("adding: {}", path.display()); })?;
//!
//! let archive_type = archive.format.clone();
//!
//! // always call `.finish()` when finished with the archive
//! // this will destroy the object
//! archive.finish()?;
//!
//! // you can then extract it:
//! let errs = archive::extract(zip_file, "foobarbaz", format, |path| { println!("extracting: {}", path.display()); })?;
//!
//! for err in errs {
//!     eprintln!("{}", err);
//! }
//!
//! // list files in archive
//! for files in archive::list(zip_file, format)? {
//!     println!("{}", files.display())
//! }
//! ```

use std::{
    fs::{File, OpenOptions},
    path::{Path, PathBuf},
};

use zip::write::SimpleFileOptions;

mod extract;
mod list;
mod write;

pub use extract::*;
pub use list::*;

/// All the types of archives available
#[derive(Debug, Clone, Copy)]
pub enum ArchiveFormat {
    /// Zip archive format
    Zip,

    /// Tarball archive format (no compression)
    Tar,
}

enum ArchiveType {
    Zip {
        writer: zip::ZipWriter<File>,
        options: SimpleFileOptions,
    },
    Tar {
        builder: tar::Builder<File>,
    },
}

/// A struct representing an archive
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::archive;
///
/// let zip_file = "foo.zip";
///
/// let mut archive = archive::Archive::new(zip_file, fs::archive::ArchiveFormat::Zip)?;
///
/// // add files:
/// archive.add_file("foo.txt", |path| { println!("adding: {}", path.display()); )?;
/// archive.add_file("bar.txt", |path| { println!("adding: {}", path.display()); )?;
/// archive.add_file("baz.txt", |path| { println!("adding: {}", path.display()); )?;
/// // or
/// archive.add_file_many([
///  "foo.txt",
///  "bar.txt",
///  "baz.txt",
/// ], |path| { println!("adding: {}", path.display()); })?;
///
/// // add directories:
/// archive.add_directory("foo", |path| { println!("adding: {}", path.display()); })?;
/// archive.add_directory("bar", |path| { println!("adding: {}", path.display()); })?;
/// archive.add_directory("baz", |path| { println!("adding: {}", path.display()); })?;
/// // or
/// archive.add_directory_many([
///  "foo",
///  "bar",
///  "baz",
/// ], |path| { println!("adding: {}", path.display()); })?;
///
/// let archive_type = archive.format.clone();
///
/// // always call `.finish()` when finished with the archive
/// // this will destroy the object
/// archive.finish()?;
///
/// // you can then extract it:
/// let errs = archive::extract(zip_file, "foobarbaz", format, |path| { println!("extracting: {}", path.display()); })?;
///
/// for err in errs {
///     eprintln!("{}", err);
/// }
///
/// // list files in archive
/// for files in archive::list(zip_file, format)? {
///     println!("{}", files.display())
/// }
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
    ///
    /// If the archive already exists it will append files to it
    pub fn new<P>(path: P, format: ArchiveFormat) -> anyhow::Result<Self>
    where
        P: AsRef<Path>,
    {
        let path = crate::fs::utility::expand_path(path);

        let exists = path.exists();

        let file;

        if exists {
            file = OpenOptions::new().read(true).write(true).open(&path)?;
        } else {
            file = File::create(&path)?;
        }

        let archive: ArchiveType = match format {
            ArchiveFormat::Zip => {
                let writer;
                let options = SimpleFileOptions::default();

                if exists {
                    writer = zip::ZipWriter::new_append(file)?;
                } else {
                    writer = zip::ZipWriter::new(file);
                }

                ArchiveType::Zip { writer, options }
            }
            ArchiveFormat::Tar => ArchiveType::Tar {
                builder: tar::Builder::new(file),
            },
        };

        Ok(Self {
            path,
            format,
            archive,
        })
    }

    /// Finalize the archive and closes it completely
    pub fn finish(self) -> anyhow::Result<()> {
        match self.archive {
            ArchiveType::Zip { writer, .. } => {
                writer.finish()?;
            }
            ArchiveType::Tar { mut builder, .. } => {
                builder.finish()?;
            }
        }

        Ok(())
    }
}
