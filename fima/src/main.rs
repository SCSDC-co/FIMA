use libfima::{fs, fs::metadata};

mod archive;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("libfima version: {}\n", libfima::VERSION);

    let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;

    for file in [
        "~/Pictures/pfp/sckab-192.png",
        "~/audio/Sfera Ebbasta - XDVR (Prod. Charlie Charles).mp3",
        "~/Videos/2025-05-21 22-12-36.mp4",
        "justfile",
        "~/Documents/PAG_2025_DMCGLN09P11A515K.pdf",
        "~/Videos",
        "fima/src/main.rs",
        "libfima/Cargo.lock",
    ] {
        println!("item: {file}");
        println!("  MIME: {}", magic.mime_type(file)?);
        println!("  Encoding: {}", magic.encoding(file)?);
        println!("  Binary: {}", magic.is_binary(file)?);
        println!("  Is media: {}", magic.is_media(file)?);
        println!("  Is video: {}", magic.is_video(file)?);
        println!("  Is audio: {}", magic.is_audio(file)?);
        println!("  Is image: {}", magic.is_image(file)?);
        println!("  Is lockfile: {}", magic.is_lockfile(file)?);
        println!(
            "  Expanded path: {}",
            fs::utility::expand_path(file).display()
        );
        println!(
            "  Permissions: {}",
            fs::metadata::Permissions::new(file)?.display(false)?
        );
        println!()
    }

    archive::setup()?;

    archive::test_zip()?;

    println!("\n");

    archive::test_tar()?;

    println!("\n");

    archive::test_xz()?;

    println!("\n");

    archive::test_gzip()?;

    println!("\n");

    archive::test_bzip()?;

    Ok(())
}
