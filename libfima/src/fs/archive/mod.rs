use std::{
    fs::File,
    path::{Path, PathBuf},
};

mod extract;
mod write;

/// All the types of archives available
#[derive(Debug)]
pub enum ArchiveFormat {
    /// Zip archive format
    Zip,
}

#[derive(Debug)]
pub enum ArchiveType {
    Zip { zip: zip::ZipWriter<File> },
}

#[derive(Debug)]
struct Archive {
    pub format: ArchiveFormat,
    pub path: PathBuf,

    archive: ArchiveType,
}

impl Archive {
    pub fn new<P, K>(path: P, format: ArchiveFormat, items: &[K]) -> anyhow::Result<Archive>
    where
        P: AsRef<Path>,
        K: AsRef<Path>,
    {
        let path = path.as_ref();

        let file = File::create(path)?;

        let archive: ArchiveType = match format {
            ArchiveFormat::Zip => ArchiveType::Zip {
                zip: zip::ZipWriter::new(file),
            },
        };

        for item in items {
            let item = item.as_ref();

            todo!()
        }

        Ok(Archive {
            path: PathBuf::from(path),
            format,
            archive,
        })
    }
}
