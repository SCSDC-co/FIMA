use anyhow::Context;
use magic::cookie;
use std::collections::HashSet;
use std::ffi::OsStr;
use std::path::Path;
use std::sync::LazyLock;

/// A module that exposes the libmagic flags
pub mod magic_flags {
    pub use magic::cookie::Flags;
}

/// These are all the lock files names, they are a LazyLock so we have only one for thread
///
/// File names:
///
/// - `package-lock.json`,
/// - `pnpm-lock.yaml`,
/// - `bun.lockb`,
/// - `.terraform.lock.hcl`,
/// - `packages.lock.json`,
/// - `gradle.lockfile`,
/// - `go.sum`,
/// - `package.resolved`,
/// - `cartfile.resolved`,
/// - `packages-lock.json`,
/// - `manifest.toml`,
/// - `cabal.project.freeze`,
/// - `opam.locked`,
/// - `vcpkg-lock.json`,
/// - `biome-lock.json`
pub static LOCK_FILES: LazyLock<HashSet<&OsStr>> = LazyLock::new(|| {
    HashSet::from([
        OsStr::new("package-lock.json"),
        OsStr::new("pnpm-lock.yaml"),
        OsStr::new("bun.lockb"),
        OsStr::new(".terraform.lock.hcl"),
        OsStr::new("packages.lock.json"),
        OsStr::new("gradle.lockfile"),
        OsStr::new("go.sum"),
        OsStr::new("package.resolved"),
        OsStr::new("cartfile.resolved"),
        OsStr::new("packages-lock.json"),
        OsStr::new("manifest.toml"),
        OsStr::new("cabal.project.freeze"),
        OsStr::new("opam.locked"),
        OsStr::new("vcpkg-lock.json"),
        OsStr::new("biome-lock.json"),
    ])
});

/// A struct that represents a `Magic` object that you can use it to make every sort of `libmagic`
/// operations
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libgima::fs::metadata;
///
/// // The variable must be `mut` as the methods will change the flags of it
/// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
///
/// magic.mime_type("foo.txt")?; // "text/plain"
/// magic.encoding("foo.txt")?; // "us-ascii"
/// ```
#[derive(Debug)]
pub struct Magic {
    cookie: magic::Cookie<cookie::Load>,
    flags: cookie::Flags,
}

impl Magic {
    /// Creates a new `Magic` object with the flags that you set, the database will be the default one
    pub fn new(flags: cookie::Flags) -> anyhow::Result<Self> {
        let cookie = magic::Cookie::open(flags)?;

        let cookie = cookie
            .load(&cookie::DatabasePaths::default())
            .map_err(|err| anyhow::anyhow!("{err}"))
            .with_context(|| "failed to load libmagic database")?;

        Ok(Magic { cookie, flags })
    }

    /// Returns the current flags of the cookie
    pub fn flags(&self) -> cookie::Flags {
        self.flags
    }

    /// Sets new flags and return old ones
    ///
    /// If the passed flags are the same as the current ones they will not be changed and the
    /// function will return the current flags
    pub fn set_flags(
        &mut self,
        flags: cookie::Flags,
    ) -> Result<cookie::Flags, cookie::SetFlagsError> {
        let old_flags = self.flags;

        if flags != old_flags {
            self.cookie.set_flags(flags)?;

            self.flags = flags;
        }

        Ok(old_flags)
    }

    /// Executes the `file` operation to an item
    ///
    /// It uses the current `Magic::flags()`
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libgima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::MIME)?;
    ///
    /// magic.file("foo.txt")?; // "text/plain; charset=us-ascii"
    /// ```
    pub fn file<P>(&self, path: P) -> Result<String, cookie::Error>
    where
        P: AsRef<Path>,
    {
        Ok(self.cookie.file(crate::fs::utility::expand_path(path))?)
    }

    /// Runs `Magic::file` with custom flags
    pub fn file_with_flags<P>(&mut self, path: P, flags: cookie::Flags) -> anyhow::Result<String>
    where
        P: AsRef<Path>,
    {
        let old_flags = self.set_flags(flags)?;

        let result = self.file(path);

        self.set_flags(old_flags)?;

        Ok(result?)
    }

    /// Returns the MIME type of an item
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libgima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// magic.mime_type("foo.txt")?; // "text/plain"
    /// magic.mime_type("foo")?; // "inode/directory"
    /// ```
    pub fn mime_type<P>(&mut self, path: P) -> anyhow::Result<String>
    where
        P: AsRef<Path>,
    {
        self.file_with_flags(path, cookie::Flags::MIME_TYPE)
    }

    /// Returns the encoding of an item:
    ///
    /// ```rust,no_run,ignore
    /// use libgima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// magic.encoding("foo.txt")?; // "us-ascii"
    /// magic.encoding("foo")?; // "binary"
    /// ```
    pub fn encoding<P>(&mut self, path: P) -> anyhow::Result<String>
    where
        P: AsRef<Path>,
    {
        self.file_with_flags(path, cookie::Flags::MIME_ENCODING)
    }

    /// Checks if a file is binary
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libfima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// magic.is_binary("foo.txt")?; // false
    /// magic.is_binary("bar.mp3")?; // true
    /// ```
    pub fn is_binary<P>(&mut self, path: P) -> anyhow::Result<bool>
    where
        P: AsRef<Path>,
    {
        Ok(self.encoding(path)? == "binary")
    }

    /// Checks if a path is a video file
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libfima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// megic.is_media("video.mp4")?; // true
    /// megic.is_media("file.txt")?; // false
    /// ```
    pub fn is_video<P>(&mut self, path: P) -> anyhow::Result<bool>
    where
        P: AsRef<Path>,
    {
        Ok(self.mime_type(path)?.starts_with("video/"))
    }

    /// Checks if a path is an audio file
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libfima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// megic.is_media("audio.mp3")?; // true
    /// megic.is_media("file.txt")?; // false
    /// ```
    pub fn is_audio<P>(&mut self, path: P) -> anyhow::Result<bool>
    where
        P: AsRef<Path>,
    {
        Ok(self.mime_type(path)?.starts_with("audio/"))
    }

    /// Checks if a path is an image
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libfima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// megic.is_media("image.png")?; // true
    /// megic.is_media("file.txt")?; // false
    /// ```
    pub fn is_image<P>(&mut self, path: P) -> anyhow::Result<bool>
    where
        P: AsRef<Path>,
    {
        Ok(self.mime_type(path)?.starts_with("image/"))
    }

    /// Checks if a path is a media (audio, video or an image)
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libfima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// megic.is_media("image.png")?; // true
    /// megic.is_media("video.mp4")?; // true
    /// megic.is_media("audio.mp3")?; // true
    /// megic.is_media("file.txt")?; // false
    /// ```
    pub fn is_media<P>(&mut self, path: P) -> anyhow::Result<bool>
    where
        P: AsRef<Path>,
    {
        Ok(self.is_video(&path)? || self.is_image(&path)? || self.is_audio(&path)?)
    }

    /// Checks if a file is a lock file
    ///
    /// It checks if the extension is ".lock" or if it is part of a predefined set of lock files
    /// file names ([`LOCK_FILES`])
    ///
    /// # Examples
    ///
    /// ```rust,no_run,ignore
    /// use libfima::fs::metadata;
    ///
    /// let mut magic = metadata::Magic::new(metadata::magic_flags::Flags::empty())?;
    ///
    /// magic.is_lock_file("Cargo.lock")?; // true
    /// magic.is_lock_file("package-lock.json")?; // true
    /// magic.is_lock_file("justfile")?; // false
    /// ```
    pub fn is_lock_file<P>(&mut self, path: P) -> anyhow::Result<bool>
    where
        P: AsRef<Path>,
    {
        let path = path.as_ref();

        Ok(LOCK_FILES.contains(path.file_name().unwrap()) || path.extension().unwrap() == "lock")
    }
}
