use std::{io, path::Path};

/// Creates one file
pub fn create_file<P>(path: P) -> Result<(), io::Error>
where
    P: AsRef<Path>,
{
    std::fs::File::create_new(crate::fs::utility::expand_path(path))?;

    Ok(())
}

/// Creates many file
pub fn create_file_many<P>(paths: &[P]) -> Result<(), io::Error>
where
    P: AsRef<Path>,
{
    for path in paths {
        std::fs::File::create_new(crate::fs::utility::expand_path(path))?;
    }

    Ok(())
}

/// Creates a directory (without creating the parents)
pub fn create_dir<P>(path: P) -> Result<(), io::Error>
where
    P: AsRef<Path>,
{
    std::fs::create_dir(crate::fs::utility::expand_path(path))?;

    Ok(())
}

/// Creates many directories (without creating the parents)
pub fn create_dir_many<P>(paths: &[P]) -> Result<(), io::Error>
where
    P: AsRef<Path>,
{
    for path in paths {
        std::fs::create_dir(crate::fs::utility::expand_path(path))?;
    }

    Ok(())
}

/// Creates a directory (and the parents)
pub fn create_dir_all<P>(path: P) -> Result<(), io::Error>
where
    P: AsRef<Path>,
{
    std::fs::create_dir_all(crate::fs::utility::expand_path(path))?;

    Ok(())
}

/// Creates many directories (and the parents)
pub fn create_dir_all_many<P>(paths: &[P]) -> Result<(), io::Error>
where
    P: AsRef<Path>,
{
    for path in paths {
        std::fs::create_dir_all(crate::fs::utility::expand_path(path))?;
    }

    Ok(())
}
