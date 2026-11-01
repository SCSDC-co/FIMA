use std::{
    fmt,
    io::{self, Read},
    path::Path,
};

use sha2::Digest;

mod hash_type;

pub use hash_type::*;

// helper function to not duplicate logic
fn read_chunks<R, F>(mut reader: R, mut update: F) -> io::Result<()>
where
    R: Read,
    F: FnMut(&[u8]),
{
    let mut buffer = [0u8; 8192];

    loop {
        let bytes_read = reader.read(&mut buffer)?;

        if bytes_read == 0 {
            break;
        }

        update(&buffer[..bytes_read]);
    }

    Ok(())
}

/// A struct that represents a hash
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Hash {
    /// The algorithm used in the hash
    pub algorithm: HashAlgorithm,

    /// The output of the hash
    pub digest: Vec<u8>,
}

impl Hash {
    /// Creates an Hash object from raw bytes
    pub fn from_bytes<K>(bytes: K, algorithm: HashAlgorithm) -> Self
    where
        K: AsRef<[u8]>,
    {
        let digest = match algorithm {
            HashAlgorithm::Sha256 => sha2::Sha256::digest(bytes).to_vec(),
            HashAlgorithm::Sha512 => sha2::Sha512::digest(bytes).to_vec(),
            HashAlgorithm::Blake3 => blake3::hash(bytes.as_ref()).as_bytes().to_vec(),
            HashAlgorithm::Md5 => md5::compute(bytes).to_vec(),
        };

        Self { algorithm, digest }
    }

    /// Creates a [`Hash`] from a reader
    ///
    /// Uses incremental hashing to avoid loading the entire input into memory
    pub fn from_reader<R>(reader: R, algorithm: HashAlgorithm) -> io::Result<Self>
    where
        R: Read,
    {
        let digest: Vec<u8>;

        match algorithm {
            HashAlgorithm::Sha256 => {
                let mut hasher = sha2::Sha256::new();

                read_chunks(reader, |chunk| hasher.update(chunk))?;

                digest = hasher.finalize().to_vec();
            }
            HashAlgorithm::Sha512 => {
                let mut hasher = sha2::Sha512::new();

                read_chunks(reader, |chunk| hasher.update(chunk))?;

                digest = hasher.finalize().to_vec();
            }
            HashAlgorithm::Blake3 => {
                let mut hasher = blake3::Hasher::new();

                read_chunks(reader, |chunk| {
                    hasher.update(chunk);
                })?;

                digest = hasher.finalize().as_bytes().to_vec();
            }
            HashAlgorithm::Md5 => {
                let mut hasher = md5::Context::new();

                read_chunks(reader, |chunk| hasher.consume(chunk))?;

                digest = hasher.finalize().0.to_vec();
            }
        }

        Ok(Self { algorithm, digest })
    }

    /// Creates a [`Hash`] from a file
    ///
    /// Internally uses [`Hash::from_reader`] to process the file incrementally
    pub fn from_file<P>(path: P, algorithm: HashAlgorithm) -> io::Result<Self>
    where
        P: AsRef<Path>,
    {
        let path = crate::fs::utility::expand_path(path);
        let file = std::fs::File::open(path)?;

        Self::from_reader(file, algorithm)
    }
}

impl fmt::Display for Hash {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}: ", self.algorithm)?;

        for byte in &self.digest {
            write!(f, "{byte:02x}")?;
        }

        Ok(())
    }
}
