/*
 * src/utility/file.cpp
 * include/utility/file.h
 *
 * A module for doing general file operations
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <ftxui/dom/node.hpp>
#include <unordered_set>

namespace fima {

namespace fs {

namespace operations {

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

inline const std::unordered_set<std::string> text_mimes{
    "application/json",         "application/xml",           "application/javascript",
    "application/x-javascript", "application/sql",           "application/graphql",
    "application/ld+json",      "application/manifest+json", "application/geo+json",
    "application/yaml",         "application/x-yaml",        "application/toml",
    "application/x-sh",         "application/x-httpd-php",   "application/rtf"
};

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

size_t
get_item_size(const std::filesystem::path& path);

std::string
make_size_readable(const size_t size);

std::filesystem::file_time_type
get_file_time(const std::filesystem::path& path);

std::string
get_file_owner(const std::filesystem::path& path);

std::string
get_file_type(const std::filesystem::path& path);

bool
is_file_executable(const std::filesystem::path& path);

bool
is_compressed_archive(const std::filesystem::path& path);

bool
is_media(const std::filesystem::path& path);

bool
is_root(const std::filesystem::path& path);

bool
is_lockfile(const std::filesystem::path& path);

bool
is_ancestor(const std::filesystem::path& ancestor, const std::filesystem::path& descendant);

void
create(const std::filesystem::path& path, const std::string_view& conent);

std::string
get_perms(const std::filesystem::path& item);

ftxui::Element
get_perms_tui(const std::filesystem::path& item);

} // namespace operations

} // namespace fs

} // namespace fima
