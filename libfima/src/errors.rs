use std::path::PathBuf;

#[derive(Debug)]
pub enum RemoveError {
    DirectoryNotEmpty(PathBuf),
}

impl std::fmt::Display for RemoveError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            self::RemoveError::DirectoryNotEmpty(path) => {
                write!(f, "Cannot remove {} since it's not empty.", path.display())
            }
        }
    }
}

impl std::error::Error for RemoveError {}
