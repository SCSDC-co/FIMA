use std::fmt;

/// Different types of hashing algorithms
#[derive(Debug, Default, PartialEq, Eq, Clone, Copy)]
pub enum HashAlgorithm {
    /// SHA-256 (Secure Hash Algorithm 256-bit)
    ///
    /// A cryptographic hash function from the SHA-2 family,
    /// producing a 256-bit (32-byte) digest
    ///
    /// Suitable for file integrity verification and general-purpose
    /// cryptographic hashing
    #[default]
    Sha256,

    /// SHA-512 (Secure Hash Algorithm 512-bit)
    ///
    /// A cryptographic hash function from the SHA-2 family,
    /// producing a 512-bit (64-byte) digest
    ///
    /// Offers a larger digest size than SHA-256
    Sha512,

    /// BLAKE3 cryptographic hash function
    ///
    /// Produces a 256-bit (32-byte) digest by default
    ///
    /// Designed for high performance, supporting parallel processing
    /// and incremental hashing
    Blake3,

    /// MD5 (Message Digest Algorithm 5)
    ///
    /// Produces a 128-bit (16-byte) digest
    ///
    /// Cryptographically broken and unsuitable for security-sensitive
    /// applications. Retained primarily for legacy compatibility
    /// and non-security-related file identification
    Md5,
}

impl fmt::Display for HashAlgorithm {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Sha256 => write!(f, "SHA-256"),
            Self::Sha512 => write!(f, "SHA-512"),
            Self::Blake3 => write!(f, "BLAKE3"),
            Self::Md5 => write!(f, "MD5"),
        }
    }
}
