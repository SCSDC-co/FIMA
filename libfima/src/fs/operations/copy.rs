use std::{fs::File, io, path::Path};

use walkdir::WalkDir;

use crate::fs::operations::create;

/// Copies a file into another
pub fn copy_file<P, K>(source: P, destination: K) -> io::Result<()>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let source = crate::fs::utility::expand_path(source);
    let mut destination = crate::fs::utility::expand_path(destination);

    let file_name = match source.file_name() {
        Some(n) => n,
        None => {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "Source has no file name.",
            ));
        }
    };

    if destination.is_dir() {
        destination = destination.join(file_name);
    }

    if source == destination {
        return Err(io::Error::new(
            io::ErrorKind::InvalidInput,
            "Source and destination must be different.",
        ));
    }

    let mut source_file = File::open(source)?;
    let mut destination_file = File::create(destination)?;

    io::copy(&mut source_file, &mut destination_file)?;

    Ok(())
}

/// Copies multiple files into a directory
pub fn copy_file_many<I, P, K>(paths: I, destination: K) -> io::Result<()>
where
    I: IntoIterator<Item = P>,
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let destination = crate::fs::utility::expand_path(destination);

    if !destination.is_dir() {
        if !destination.exists() {
            create::create_dir_all(&destination)?;
        } else {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "Destination must be a directory.",
            ));
        }
    }

    for path in paths {
        let path = crate::fs::utility::expand_path(path);

        let file_name = match path.file_name() {
            Some(n) => n,
            None => {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidInput,
                    format!("Path {} has no file name.", path.display()),
                ));
            }
        };

        copy_file(&path, destination.join(file_name))?;
    }

    Ok(())
}

/// Copies a dir into an another
pub fn copy_dir<P, K>(source: P, destination: K) -> anyhow::Result<()>
where
    P: AsRef<Path>,
    K: AsRef<Path>,
{
    let source = crate::fs::utility::expand_path(source);
    let destination = crate::fs::utility::expand_path(destination);

    for item in WalkDir::new(&source) {
        let entry = item?;
        let is_dir = entry.file_type().is_dir();
        let item_path = entry.into_path();

        let source_parent = source.parent().unwrap();
        let relative_path = item_path.strip_prefix(source_parent)?;

        let item = destination.join(relative_path);

        if is_dir {
            create::create_dir(item)?;
        } else {
            let mut source_file = File::open(item_path)?;
            let mut destination_file = File::create(item)?;

            io::copy(&mut source_file, &mut destination_file)?;
        }
    }

    Ok(())
}
