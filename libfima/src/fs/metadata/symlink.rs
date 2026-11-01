use std::path::{Path, PathBuf};
use std::{fmt, fs, io};

use path_clean::PathClean;

#[derive(Debug, PartialEq, Eq, Clone)]
/// Types of symlinks
///
/// Unix-like systems will treat both types the exact the same way
pub enum SymlinkType {
    /// A symlink to a file
    File,

    /// A symlink to a directory
    Dir,
}

#[derive(Debug, PartialEq, Clone)]
/// A structure to represent symlinks
pub struct Symlink {
    /// The path of the symlink
    pub path: PathBuf,

    /// The target that the symlink points to
    pub target: PathBuf,

    symlink_type: SymlinkType,
}

impl Symlink {
    /// Creates a new Symlink objects, it does not create the symlink in the filesystem
    pub fn new<P, K>(path: P, target: K, symlink_type: SymlinkType) -> Self
    where
        P: AsRef<Path>,
        K: AsRef<Path>,
    {
        Self {
            path: crate::fs::utility::expand_path(path),
            target: crate::fs::utility::expand_path(target),
            symlink_type,
        }
    }

    /// Creates a Symlink struct from an already existing symlink
    pub fn from_path<P>(path: P) -> io::Result<Self>
    where
        P: AsRef<Path>,
    {
        let path = crate::fs::utility::expand_path(path);

        let symlink_type: SymlinkType;

        if fs::symlink_metadata(&path)?.is_dir() {
            symlink_type = SymlinkType::Dir;
        } else {
            symlink_type = SymlinkType::File;
        }

        Ok(Self {
            path: path.clone(),
            target: fs::read_link(path)?,
            symlink_type,
        })
    }

    /// Creates the symlink in the filesystem
    pub fn create(&self) -> io::Result<()> {
        match &self.symlink_type {
            SymlinkType::File => symlink::symlink_file(&self.path, &self.target),
            SymlinkType::Dir => symlink::symlink_dir(&self.path, &self.target),
        }
    }

    /// Creates the target
    pub fn create_target(&self) -> io::Result<()> {
        let parent = match self.path.parent() {
            Some(p) => p.clean(),
            None => {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidInput,
                    "Symlink has no parent directory",
                ));
            }
        };

        let target = if self.target.is_absolute() {
            self.target.clone()
        } else {
            parent.join(&self.target)
        };

        let target = target.clean();

        match &self.symlink_type {
            SymlinkType::File => crate::fs::operations::create::create_file(target),
            SymlinkType::Dir => crate::fs::operations::create::create_dir_all(target),
        }
    }

    /// Reads the target that the symlink points to
    pub fn read_target(&self) -> io::Result<PathBuf> {
        fs::read_link(&self.path)
    }

    /// Resolve the symlink target
    pub fn resolve(&self) -> io::Result<PathBuf> {
        let parent = match self.path.parent() {
            Some(p) => p.canonicalize()?,
            None => {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidInput,
                    "Symlink has no parent directory",
                ));
            }
        };

        let target = if self.target.is_absolute() {
            self.target.clone()
        } else {
            parent.join(&self.target)
        };

        target.canonicalize()
    }

    /// Remove the symlink
    pub fn remove(&self) -> io::Result<()> {
        match &self.symlink_type {
            SymlinkType::File => symlink::remove_symlink_file(&self.path),
            SymlinkType::Dir => symlink::remove_symlink_dir(&self.path),
        }
    }

    /// Checks if the symlink is valid
    pub fn is_valid(&self) -> io::Result<bool> {
        if !fs::symlink_metadata(&self.path)?.file_type().is_symlink() {
            return Ok(false);
        }

        Ok(self.path.exists())
    }
}

impl fmt::Display for Symlink {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{} -> {}", self.path.display(), self.target.display())
    }
}
