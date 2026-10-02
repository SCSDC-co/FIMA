use libfima::fs;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("libfima version: {}", libfima::VERSION);

    let mut magic = fs::metadata::Magic::new(fs::metadata::magic_flags::Flags::empty())?;

    let file = "justfile";

    println!("file: {file}");
    println!("  MIME: {}", magic.mime_type(file)?);
    println!("  encoding: {}", magic.encoding(file)?);

    Ok(())
}
