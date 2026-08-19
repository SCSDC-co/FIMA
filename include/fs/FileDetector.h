/*
 * src/fs/FileDetector.cpp
 * include/fs/FileDetector.h
 *
 * A RAII wrapper around libmagic
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <unordered_set>

#ifndef _WIN32
#  include <magic.h>
#endif

namespace fima {

namespace fs {

inline const std::unordered_set<std::string> lockfiles{
    "package-lock.json",  "pnpm-lock.yaml",     "bun.lockb",      ".terraform.lock.hcl",
    "packages.lock.json", "gradle.lockfile",    "go.sum",         "package.resolved",
    "cartfile.resolved",  "packages-lock.json", "manifest.toml",  "cabal.project.freeze",
    "opam.locked",        "vcpkg-lock.json",    "biome-lock.json"
};

inline const std::unordered_set<std::string> executables = {
    ".exe", ".dll",   ".sys", ".cpl", ".ocx",      ".scr", ".efi",  ".msi",
    ".app", ".apk",   ".ipa", ".elf", ".o",        ".obj", ".com",  ".bin",
    ".so",  ".dylib", ".out", ".run", ".appimage", ".ko",  ".wasm", ".pyc",
};

inline const std::unordered_set<std::string> scripts = {
    ".sh",  ".zsh", ".bash", ".csh", ".ksh", ".tcsh", ".fish",        ".py", ".pyw",
    ".pl",  ".pm",  ".rb",   ".php", ".js",  ".mjs",  ".cjs",         ".ts", ".lua",
    ".ps1", ".vbs", ".bat",  ".cmd", ".awk", ".tcl",  ".applescript",
};

#ifndef _WIN32
inline const std::unordered_set<std::string> text_mimes{
    "application/json",         "application/xml",           "application/javascript",
    "application/x-javascript", "application/sql",           "application/graphql",
    "application/ld+json",      "application/manifest+json", "application/geo+json",
    "application/yaml",         "application/x-yaml",        "application/toml",
    "application/x-sh",         "application/x-httpd-php",   "application/rtf"
};

inline const std::unordered_set<std::string_view> archive_mimes{
    "application/zip",
    "application/vnd.rar",
    "application/x-7z-compressed",
    "application/x-tar",
    "application/gzip",
    "application/x-bzip2",
    "application/x-xz",
    "application/x-compress",
    "application/x-lzip",
    "application/x-lzma",
    "application/x-lzop",
    "application/zstd",
    "application/vnd.apple.installer+xml",
    "application/x-apple-diskimage",
    "application/x-cab",
    "application/x-msi",
    "application/x-iso9660-image",
    "application/x-apple-diskimage",
    "application/vnd.debian.binary-package",
    "application/x-rpm",
    "application/vnd.snap",
    "application/java-archive",
    "application/zip",
    "application/zip",
    "application/zip",
    "application/vnd.android.package-archive",
    "application/octet-stream",
    "application/zip",
    "application/python-egg",
    "application/gzip",
    "application/x-xz",
    "application/zstd",
    "application/x-lz4",
    "application/brotli",
    "application/x-iso9660-image",
    "application/x-raw-disk-image",
    "application/x-archive",
    "application/x-cpio",
    "application/vnd.crate",
    "application/x-ace-compressed",
    "application/x-arc",
    "application/x-arj",
};
#else
inline const std::unordered_set<std::string> media{
    ".jpg",  ".jpeg", ".png", ".bmp",  ".tiff", ".tif",  ".webp", ".avif", ".heif", ".heic", ".ico",
    ".cur",  ".psd",  ".xcf", ".raw",  ".cr2",  ".nef",  ".arw",  ".dng",  ".gif",  ".mp4",  ".mkv",
    ".avi",  ".mov",  ".wmv", ".flv",  ".mpeg", ".mpg",  ".3gp",  ".3g2",  ".m2ts", ".vob",  ".ogv",
    ".rm",   ".rmvb", ".asf", ".divx", ".hevc", ".h264", ".h265", ".f4v",  ".mxf",  ".roq",  ".drc",
    ".amv",  ".webm", ".m4v", ".mp3",  ".aac",  ".m4a",  ".ogg",  ".opus", ".wma",  ".amr",  ".ac3",
    ".flac", ".alac", ".wav", ".aiff", ".aif",  ".ape",  ".wv",   ".tta",  ".oga",  ".mka",  ".ra",
    ".mid",  ".midi", ".rmi", ".dsf",  ".dff",  ".caf",  ".pcm",  ".jxl",  ".jfif", ".jpe",  ".svg",
    ".svgz", ".tga",  ".exr", ".hdr",  ".bpg",  ".qoi",  ".pbm",  ".pgm",  ".ppm",  ".pnm",  ".m2v",
    ".ogm",  ".y4m",  ".m4b", ".m4p",  ".m4r",  ".weba", ".spx",  ".au",   ".snd",  ".voc",  ".w64",
    ".mpc",  ".shn",  ".tak", ".xm",   ".mod",  ".s3m",  ".it",   ".aifc", ".mp1",  ".mp2",
};

inline const std::unordered_set<std::string> archives{
    ".zip", ".rar", ".7z",  ".tar", ".gz",  ".bz2",  ".xz",    ".z",   ".lz",   ".lzma", ".lzo",
    ".zst", ".dmg", ".pkg", ".xip", ".cab", ".msi",  ".wim",   ".deb", ".rpm",  ".snap", ".jar",
    ".war", ".ear", ".aar", ".apk", ".ipa", ".whl",  ".egg",   ".tgz", ".tbz2", ".txz",  ".tzst",
    ".lz4", ".br",  ".iso", ".img", ".ar",  ".cpio", ".crate", ".ace", ".arc",  ".arj",
};
#endif

class FileDetector
{
  public:
#ifndef _WIN32
#  ifndef FIMA_MAGIC_DATABASE
#    error "FIMA_MAGIC_DATABASE is not defined"
#  endif
    FileDetector()
      : magic(magic_open(MAGIC_MIME_TYPE | MAGIC_SYMLINK))
    {
        if (this->magic == nullptr) {
            throw std::runtime_error("Failed to initialize libmagic");
        }

        const char* magic_database = std::getenv("MAGIC");

        if (magic_load(this->magic, magic_database ? magic_database : FIMA_MAGIC_DATABASE) == -1) {
            throw std::runtime_error(std::string("Magic: ") + magic_error(this->magic) + '\n' +
                                     std::string("FIMA_MAGIC_DATABASE: ") + FIMA_MAGIC_DATABASE);
        }
    }

    ~FileDetector()
    {
        if (this->magic != nullptr) {
            magic_close(this->magic);
        }
    }
#endif

    FileDetector(const FileDetector&)            = delete;
    FileDetector& operator=(const FileDetector&) = delete;

    [[nodiscard]] bool is_file_clocable(const std::filesystem::path& path);

    [[nodiscard]] bool is_file_executable(const std::filesystem::path& path);
    [[nodiscard]] bool is_file_media(const std::filesystem::path& path);
    [[nodiscard]] bool is_file_lockfile(const std::filesystem::path& path);
    [[nodiscard]] bool is_file_binary(const std::filesystem::path& path);
    [[nodiscard]] bool is_compressed_archive(const std::filesystem::path& path);

    [[nodiscard]] std::string get_encoding(const std::filesystem::path& path);
    [[nodiscard]] std::string get_mime(const std::filesystem::path& path);

  private:
#ifndef _WIN32
    magic_t magic{};

    std::string run_file_with_flags(const int& flags, const std::filesystem::path& path)
    {
        int old_flags{ magic_getflags(this->magic) };

        magic_setflags(this->magic, flags);

        const char* result{ magic_file(this->magic, path.c_str()) };

        magic_setflags(this->magic, old_flags);

        return (result != nullptr ? result : "-1");
    }
#endif
};

extern FileDetector file_detector;
} // namespace fs

} // namespace fima
