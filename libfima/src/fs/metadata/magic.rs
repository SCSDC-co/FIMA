use anyhow::Context;
use magic::cookie;
use magic::cookie::Flags;
use std::path::Path;

pub struct Magic {
    cookie: magic::Cookie<cookie::Load>,
    flags: Flags,
}

impl Magic {
    pub fn new(default_flags: Flags) -> anyhow::Result<Magic> {
        let cookie = magic::Cookie::open(default_flags)?;

        let cookie = cookie
            .load(&cookie::DatabasePaths::default())
            .map_err(|err| anyhow::anyhow!("{err}"))
            .with_context(|| "failed to load libmagic database")?;

        Ok(Magic {
            cookie,
            flags: default_flags,
        })
    }

    pub fn mime_type<P>(&mut self, path: P) -> anyhow::Result<String>
    where
        P: AsRef<Path>,
    {
        let old_flags = self.flags;

        self.cookie.set_flags(Flags::MIME_TYPE)?;

        let mime_type = self.cookie.file(path)?;

        self.cookie.set_flags(old_flags)?;

        Ok(mime_type)
    }
}
