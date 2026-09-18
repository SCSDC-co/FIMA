use std::fs;
use std::path::PathBuf;

use crate::errors;

pub fn remove(path: PathBuf, opts: super::RemoveOptions) -> Result<(), Box<dyn std::error::Error>> {
    if path.is_dir() {
        if path.is_empty() {
            fs::remove_dir(path)?;
        } else {
            if opts.recursive {
                fs::remove_dir_all(path)?;
            } else {
                return Err(errors::RemoveError::DirectoryNotEmpty(path).into());
            }
        }
    } else {
        fs::remove_file(path)?;
    }

    Ok(())
}
