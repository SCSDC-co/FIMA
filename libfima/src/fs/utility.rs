use std::fs;
use std::io::Error;
use std::path::Path;

pub use glob;

pub fn is_dir_empty(dir: &Path) -> Result<bool, Error> {
    Ok(fs::read_dir(dir)?.next().is_none())
}
