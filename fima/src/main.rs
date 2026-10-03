use libfima::fs::metadata;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("libfima version: {}", libfima::VERSION);

    let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;

    for file in [
        "~/Pictures/pfp/sckab-192.png",
        "~/audio/Sfera Ebbasta - XDVR (Prod. Charlie Charles).mp3",
        "~/Videos/2025-05-21 22-12-36.mp4",
        "justfile",
        "~/Documents/PAG_2025_DMCGLN09P11A515K.pdf",
    ] {
        println!("file: {file}");
        println!("  MIME: {}", magic.mime_type(file)?);
        println!("  Encoding: {}", magic.encoding(file)?);
        println!("  Binary: {}", metadata::is_binary(&mut magic, file)?);
        println!()
    }

    Ok(())
}
