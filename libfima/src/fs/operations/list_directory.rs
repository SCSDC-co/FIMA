use std::{
    cmp::Ordering,
    fs, io,
    path::{Path, PathBuf},
};

/// Defines the sorting strategy used when listing directory entries
#[derive(Default)]
pub enum Sorting {
    /// Sorts entries alphabetically by name
    #[default]
    Alphabetical,

    /// Places directories before files
    DirectoriesFirst,

    /// Places directories after files
    DirectoriesLast,

    /// Places directories first and sorts entries alphabetically
    AlphabeticalDirectoriesFirst,

    /// Places directories last and sorts entries alphabetically
    AlphabeticalDirectoriesLast,

    /// Uses a custom sorting function
    ///
    /// The function receives two paths and returns their relative ordering
    Custom(Box<dyn FnMut(&Path, &Path) -> Ordering>),
}

/// Configuration options for listing directory entries
pub struct Options {
    /// Sorting strategy applied to the directory entries
    pub sorting: Sorting,

    /// Whether hidden files and directories should be included
    pub hidden_files: bool,

    /// Optional filter used to determine which entries are included
    ///
    /// Entries are included when the function returns `true`
    pub filter: Option<Box<dyn Fn(&Path) -> bool>>,
}
/// Returns the entries contained in a directory
///
/// Applies the specified filtering and sorting options to the directory entries
///
/// # Examples
///
/// ```rust,no_run,ignore
/// use libfima::fs::operations::list_directory::{list_dir, Options};
///
/// let entries = list_dir(".", Options::default())?;
/// ```
pub fn list_dir<P>(dir: P, mut options: Options) -> io::Result<Vec<PathBuf>>
where
    P: AsRef<Path>,
{
    let dir = crate::fs::utility::expand_path(dir);

    let mut list = vec![];

    for item in fs::read_dir(dir)? {
        let path = item?.path();

        if !options.hidden_files && crate::fs::utility::is_file_hidden(&path)? {
            continue;
        }

        match &options.filter {
            Some(f) => {
                if !f(path.as_path()) {
                    continue;
                }
            }
            None => {}
        }

        list.push(path);
    }

    match &mut options.sorting {
        Sorting::Alphabetical => {
            list.sort_by(|a, b| a.file_name().cmp(&b.file_name()));
        }
        Sorting::DirectoriesFirst => {
            list.sort_by(|a, b| b.is_dir().cmp(&a.is_dir()));
        }
        Sorting::DirectoriesLast => {
            list.sort_by(|a, b| a.is_dir().cmp(&b.is_dir()));
        }
        Sorting::AlphabeticalDirectoriesFirst => {
            list.sort_by(|a, b| {
                b.is_dir()
                    .cmp(&a.is_dir())
                    .then_with(|| a.file_name().cmp(&b.file_name()))
            });
        }
        Sorting::AlphabeticalDirectoriesLast => {
            list.sort_by(|a, b| {
                a.is_dir()
                    .cmp(&b.is_dir())
                    .then_with(|| a.file_name().cmp(&b.file_name()))
            });
        }
        Sorting::Custom(function) => {
            list.sort_by(|a, b| function(a, b));
        }
    }

    Ok(list)
}

impl Default for Options {
    fn default() -> Self {
        Self {
            sorting: Sorting::default(),
            hidden_files: false,
            filter: None,
        }
    }
}
