<div align=center>

# libfima

[Documentation](https://docs.rs/libfima/0.1.0)

<img src="https://skillicons.dev/icons?i=rust" >

</div>

Libfima is a blazingly fast library for creating Fast, Incredible, Minimale and awesome file managers.

It is the library used in [FIMA](https://github.com/SCSDC-co/FIMA).

## Usage

Add this to your `Cargo.toml`:

```toml
[dependencies]
libfima = "0.1.3"
```

## Example

```rust
use libfima::fs;

println!("{}", libfima::VERSION);

// General information about files:
let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;

for file in [
    "~/foo/bar/baz.txt",
    "foobar/bazbar.mp3",
    "barbar",
    "bazbaz.pdf",
    "~/foobar",
    "foo/src/rust.rs",
    "foo/Cargo.lock",
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

// working with archives:
let file = "foo.zip";
let output_path = "barbar";

if std::fs::exists(file)? {
    fs::operations::remove::remove(
        file,
        &fs::operations::remove::Options { recursive: false },
    )?;
}

let mut archive = archive::Archive::new(file, fs::archive::ArchiveFormat::Zip)?;

archive.add_file_many(["foo.json", "bazbaz"])?;
archive.add_directory("foo/src")?;

let format = archive.format.clone();

archive.finish()?;

let errs = archive::extract(file, output_path, format)?;

for err in errs {
    eprintln!("{}", err);
}

for files in archive::list(file, format)? {
    println!("{}", files.display())
}
```
