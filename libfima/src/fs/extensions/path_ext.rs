// keep that first empty line, it is just to not show the content of the docs in the module overview
// of fs
//!
//! The main function of this module is to simplify the usage of libfima by adding custom methods to
//! Path and PathBuf.
//!
//! # Examples
//!
//! ```rust,no_run,ignore
//! use libfima::fs::extensions::PathExt;
//!
//! let path = Path::new("foo.txt");
//!
//! path.create()?;
//!
//! path.rename("bar.txt")?;
//!
//! path.size()?;
//! ```

use crate::fs::{metadata::Permissions, operations::remove};
use std::{
    ffi::OsStr,
    io,
    path::{Path, PathBuf},
};

/// Extension trait providing convenient filesystem operations and metadata access
/// directly on [`Path`] values
pub trait PathExt {
    /// Creates a file or directory at this path
    ///
    /// Wrapper around `create::create_dir_all` or `create::create_file`
    fn create(&self, dir: bool) -> io::Result<()>;

    /// Moves this path to the specified destination
    ///
    /// Wrapper around `rename::move_to`
    fn move_to<P>(&self, destination: P) -> io::Result<()>
    where
        P: AsRef<Path>;

    /// Copies this path to the specified destination
    ///
    /// Wrapper around `copy::copy_file`
    fn copy_to<P>(&self, destination: P) -> io::Result<()>
    where
        P: AsRef<Path>;

    /// Copies this path into a directory
    ///
    /// wrapper around `copy_file_to_dir`
    fn copy_to_dir<P>(&self, destination: P) -> io::Result<()>
    where
        P: AsRef<Path>;

    /// Removes this path using the specified options
    ///
    /// Wrapper around `remove::remove`
    fn remove(&self, options: &remove::Options) -> io::Result<()>;

    /// Renames this path to the specified name
    ///
    /// Wrapper around `rename::rename`
    fn rename<S>(&self, new_name: S) -> io::Result<()>
    where
        S: AsRef<OsStr>;

    /// Expands this path
    ///
    /// Wrapper around `utility::expand_path`
    fn expand(&self) -> PathBuf;

    /// Checks whether this path is a filesystem root
    ///
    /// Wrapper around `utility::is_root`
    fn is_root(&self) -> io::Result<bool>;

    /// Checks whether this path is an ancestor of the specified path
    ///
    /// Wrapper around `utility::is_ancestor`
    fn is_ancestor_of<P>(&self, path: P) -> bool
    where
        P: AsRef<Path>;

    /// Checks whether this path is an ancestor of the current working directory
    ///
    /// Wrapper around `utility::is_ancestor_of_cwd`
    fn is_ancestor_of_cwd(&self) -> io::Result<bool>;

    /// Checks whether this directory is empty
    ///
    /// Wrapper around `utility::is_dir_empty`
    fn is_dir_empty(&self) -> io::Result<bool>;

    /// Returns the owner of this path
    ///
    /// Wrapper around `metadata::owner`
    fn owner(&self) -> anyhow::Result<String>;

    /// Returns the group associated with this path
    ///
    /// Wrapper around `metadata::group`
    fn group(&self) -> anyhow::Result<String>;

    /// Returns the permissions of this path
    ///
    /// Wrapper around `metadata::Permissions::new`
    fn permissions(&self) -> io::Result<Permissions>;

    /// Returns the size of this path in bytes
    ///
    /// Wrapper around `metadata::size`
    fn size(&self) -> io::Result<u64>;

    /// Returns the number of items in this directory
    ///
    /// Wrapper around `metadata::item_count`
    fn item_count(&self) -> io::Result<u64>;

    /// Returns the last modification time of this path
    ///
    /// Wrapper around `metadata::last_modified_time`
    fn last_modified_time(&self) -> io::Result<std::time::SystemTime>;

    /// Returns the formatted last modification time of this path
    ///
    /// Wrapper around `metadata::last_modified_time_formatted`
    fn last_modified_time_formatted(&self) -> anyhow::Result<String>;

    /// Returns the last modification time formatted using the specified format
    ///
    /// Wrapper around `metadata::last_modified_time_formatted_custom`
    fn last_modified_time_formatted_custom<S>(&self, format: S) -> anyhow::Result<String>
    where
        S: AsRef<str>;
}

impl PathExt for Path {
    fn create(&self, dir: bool) -> io::Result<()> {
        if dir {
            crate::fs::operations::create::create_dir_all(self)
        } else {
            crate::fs::operations::create::create_file(self)
        }
    }
    fn move_to<P>(&self, destination: P) -> io::Result<()>
    where
        P: AsRef<Path>,
    {
        crate::fs::operations::rename::move_to([self], destination)
    }
    fn copy_to<P>(&self, destination: P) -> io::Result<()>
    where
        P: AsRef<Path>,
    {
        crate::fs::operations::copy::copy_file(self, destination)
    }
    fn copy_to_dir<P>(&self, destination: P) -> io::Result<()>
    where
        P: AsRef<Path>,
    {
        crate::fs::operations::copy::copy_file_to_dir(self, destination)
    }
    fn remove(&self, options: &remove::Options) -> io::Result<()> {
        crate::fs::operations::remove::remove(self, options)
    }
    fn rename<S>(&self, new_name: S) -> io::Result<()>
    where
        S: AsRef<OsStr>,
    {
        crate::fs::operations::rename::rename(self, new_name.as_ref())
    }
    fn expand(&self) -> PathBuf {
        crate::fs::utility::expand_path(self)
    }

    fn is_root(&self) -> io::Result<bool> {
        crate::fs::utility::is_root(self)
    }
    fn is_ancestor_of<P>(&self, path: P) -> bool
    where
        P: AsRef<Path>,
    {
        crate::fs::utility::is_ancestor(self, path)
    }
    fn is_ancestor_of_cwd(&self) -> io::Result<bool> {
        crate::fs::utility::is_ancestor_of_cwd(self)
    }
    fn is_dir_empty(&self) -> io::Result<bool> {
        if self.is_dir() {
            crate::fs::utility::is_dir_empty(self)
        } else {
            Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "The path is not a directory.",
            ))
        }
    }

    fn owner(&self) -> anyhow::Result<String> {
        crate::fs::metadata::owner(self)
    }
    fn group(&self) -> anyhow::Result<String> {
        crate::fs::metadata::group(self)
    }
    fn permissions(&self) -> io::Result<Permissions> {
        crate::fs::metadata::Permissions::new(self)
    }
    fn size(&self) -> io::Result<u64> {
        crate::fs::metadata::size(self)
    }
    fn item_count(&self) -> io::Result<u64> {
        crate::fs::metadata::item_count(self)
    }
    fn last_modified_time(&self) -> io::Result<std::time::SystemTime> {
        crate::fs::metadata::last_modified_time(self)
    }
    fn last_modified_time_formatted(&self) -> anyhow::Result<String> {
        crate::fs::metadata::last_modified_time_formatted(self)
    }
    fn last_modified_time_formatted_custom<S>(&self, format: S) -> anyhow::Result<String>
    where
        S: AsRef<str>,
    {
        crate::fs::metadata::last_modified_time_formatted_custom(self, format)
    }
}
