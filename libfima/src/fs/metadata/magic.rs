use anyhow::Context;
use magic::cookie;
use std::path::Path;

/// A module that exposes the libmagic flags
pub mod magic_flags {
    pub use magic::cookie::Flags;
}

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
/// let mime = magic.mime_type("foo.txt")?; // "text/plain"
/// let encoding = magic.encoding("foo.txt")?; // "us-ascii"
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
    /// let file = magic.file("foo.txt")?; // "text/plain; charset=us-ascii"
    /// ```
    pub fn file<P>(&self, path: P) -> Result<String, cookie::Error>
    where
        P: AsRef<Path>,
    {
        Ok(self.cookie.file(path)?)
    }

    /// Runs `Magic::file` with custom flags that you pass to it
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
    /// let mime = magic.mime_type("foo.txt")?; // "text/plain"
    /// let mime2 = magic.mime_type("foo")?; // "inode/directory"
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
    /// let mime = magic.encoding("foo.txt")?; // "us-ascii"
    /// let mime2 = magic.encoding("foo")?; // "binary"
    /// ```
    pub fn encoding<P>(&mut self, path: P) -> anyhow::Result<String>
    where
        P: AsRef<Path>,
    {
        self.file_with_flags(path, cookie::Flags::MIME_ENCODING)
    }
}
