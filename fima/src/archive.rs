use std::{
    io,
    path::{Path, PathBuf},
};

use libfima::fs::operations::{create, remove};
use libfima::fs::{archive, compressors};
use std::sync::LazyLock;

static ARCHIVE_DIR: LazyLock<PathBuf> = LazyLock::new(|| PathBuf::from("archive"));

pub fn setup() -> io::Result<()> {
    remove::remove(
        ARCHIVE_DIR.to_path_buf(),
        &remove::Options { recursive: true },
    )?;

    create::create_dir_all(ARCHIVE_DIR.to_path_buf())?;

    Ok(())
}

pub fn test_zip() -> Result<(), Box<dyn std::error::Error>> {
    let file = ARCHIVE_DIR.join("test.zip");
    let output_path = ARCHIVE_DIR.join("zip_extract");
    let mut count = 0;

    let mut action = |path: PathBuf| -> () {
        count += 1;
        println!("(zip) adding {}", path.display())
    };

    let mut archive = archive::Archive::new(&file, archive::ArchiveFormat::Zip)?;

    archive.add_file_many(["package.json", "justfile"], &mut action)?;
    archive.add_directory("libfima/src/fs/operations", action)?;

    let format = archive.format.clone();

    archive.finish()?;

    println!("(zip) item added: {count}");

    let errs = archive::extract(&file, output_path, format, |path| {
        println!("(zip) extracting {}", path.display())
    })?;

    for err in errs {
        eprintln!("(zip) {}", err);
    }

    println!();

    for files in archive::list(file, format)? {
        println!("(zip) {}", files.display())
    }

    Ok(())
}

pub fn test_tar() -> Result<(), Box<dyn std::error::Error>> {
    let file = ARCHIVE_DIR.join("test.tar");
    let output_path = ARCHIVE_DIR.join("tar_extract");
    let mut count = 0;

    let mut action = |path: PathBuf| -> () {
        count += 1;
        println!("(tar) adding {}", path.display())
    };

    let mut archive = archive::Archive::new(&file, archive::ArchiveFormat::Tar)?;

    archive.add_file_many(["package.json", "justfile"], &mut action)?;
    archive.add_directory("libfima/src/fs/operations", action)?;

    let format = archive.format.clone();

    archive.finish()?;

    println!("(tar) item added: {count}");

    let errs = archive::extract(&file, output_path, format, |path| {
        println!("(tar) extracting {}", path.display())
    })?;

    for err in errs {
        eprintln!("(tar) {}", err);
    }

    println!();

    for files in archive::list(file, format)? {
        println!("(tar) {}", files.display())
    }

    Ok(())
}

pub fn test_xz() -> Result<(), Box<dyn std::error::Error>> {
    let file = Path::new("package.json");
    let xz_file = ARCHIVE_DIR.join(file.with_added_extension("xz"));

    let output_dir = ARCHIVE_DIR.join("xz_output");
    create::create_dir_all(&output_dir)?;

    let output_file = output_dir.join(file);

    compressors::compress_file(file, &xz_file, compressors::CompressorFormat::Xz, 9)?;

    compressors::decompress_file(&xz_file, &output_file, compressors::CompressorFormat::Xz)?;

    Ok(())
}
