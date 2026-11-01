use std::fmt;

use sha2::Digest;

mod hash_type;

pub use hash_type::*;

/// A struct that represents an hash
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
