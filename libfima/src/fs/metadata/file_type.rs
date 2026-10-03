use std::path::Path;

pub fn is_binary<P>(magic: &mut super::Magic, path: P) -> anyhow::Result<bool>
where
    P: AsRef<Path>,
{
    Ok(magic.encoding(path)? == "binary")
}
