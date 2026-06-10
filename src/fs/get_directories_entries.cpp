/*
 * src/fs/get_directories_entries.cpp
 * include/fs/get_directories_entries.h
 *
 * This file contains the helpers for getting the entries of a directory, both recursively and not
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/get_directories_entries.h"

#include <filesystem>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

#include "fs/operations.h"
#include "utility/regex.h"

namespace _fs = std::filesystem;

namespace fima {

namespace fs {

std::vector<_fs::directory_entry>
get_directories_entries(const _fs::path& path, const fima::git::GitRepo& repo, const bool& dotfiles)
{
    std::vector<_fs::directory_entry> directory_content;

    for (const _fs::directory_entry& entry : _fs::directory_iterator(
           path, std::filesystem::directory_options::skip_permission_denied)) {
        std::string name{ entry.path().filename().string() };

        if (repo.is_file_ignored(entry.path())) {
            continue;
        }

        if (name[0] == '.' && !dotfiles) {
            continue;
        }

        directory_content.push_back(entry);
    }

    return directory_content;
}

std::vector<_fs::directory_entry>
get_directories_entries_no_git(const _fs::path& path, const bool& dotfiles)
{
    std::vector<_fs::directory_entry> directory_content;

    for (const _fs::directory_entry& entry : _fs::directory_iterator(
           path, std::filesystem::directory_options::skip_permission_denied)) {
        std::string name{ entry.path().filename().string() };

        if (name[0] == '.' && !dotfiles) {
            continue;
        }

        directory_content.push_back(entry);
    }

    return directory_content;
}

std::vector<_fs::path>
get_directories_entries_recursive(const std::filesystem::path& path,
                                  const fima::git::GitRepo& repo,
                                  const bool& ignore_directories_or_files,
                                  const std::vector<std::regex>& ignored_files_or_directories)
{
    std::vector<_fs::path> directory_content;

    auto it =
      _fs::recursive_directory_iterator(path, _fs::directory_options::skip_permission_denied);

    for (const auto& entry : it) {
        std::string name = entry.path().filename().string();

        if (repo.is_file_ignored(entry.path())) {
            it.disable_recursion_pending();
            continue;
        }

        if (ignore_directories_or_files &&
            fima::helpers::regex::matches_any_regex(name, ignored_files_or_directories)) {
            it.disable_recursion_pending();
            continue;
        }

        directory_content.push_back(entry.path());
    }

    return directory_content;
}

std::vector<_fs::path>
get_directories_entries_recursive_no_git(
  const std::filesystem::path& path,
  const bool& ignore_directories_or_files,
  const std::vector<std::regex>& ignored_files_or_directories)
{
    std::vector<_fs::path> directory_content;

    auto it =
      _fs::recursive_directory_iterator(path, _fs::directory_options::skip_permission_denied);

    for (const auto& entry : it) {
        std::string name = entry.path().filename().string();

        if (ignore_directories_or_files &&
            fima::helpers::regex::matches_any_regex(name, ignored_files_or_directories)) {
            it.disable_recursion_pending();
            continue;
        }

        directory_content.push_back(entry.path());
    }

    return directory_content;
}

std::vector<_fs::path>
get_directories_for_cloc(const _fs::path& path,
                         const fima::git::GitRepo& repo,
                         const bool& gitignore,
                         const bool& ignore,
                         const std::vector<std::regex>& ignored_files_or_directories)
{
    std::vector<_fs::path> paths{};

    auto file_is_ignored = [&](std::filesystem::path path) {
        return ignore && fima::helpers::regex::matches_any_regex(path.filename().string(),
                                                                 ignored_files_or_directories);
    };

    static const std::unordered_set<std::string> ft_to_skip = {
        ".zip",  ".tar",    ".png",      ".jpeg",  ".jpg",  ".mp3",  ".mp4",  ".mp2",   ".mp1",
        ".wav",  ".avi",    ".webp",     ".undo",  ".spl",  ".ico",  ".icns", ".mpack", ".exe",
        ".o",    ".class",  ".appimage", ".woff2", ".ttf",  ".ttf2", ".dll",  ".rar",   ".7z",
        ".gz",   ".bz2",    ".xz",       ".z",     ".lz",   ".lzma", ".lzo",  ".zst",   ".tgz",
        ".tbz2", ".tar.gz", ".tar.xz",   ".gif",   ".bmp",  ".tiff", ".tif",  ".avif",  ".heif",
        ".heic", ".svg",    ".psd",      ".xcf",   ".raw",  ".cr2",  ".nef",  ".arw",   ".dng",
        ".mkv",  ".webm",   ".mov",      ".wmv",   ".flv",  ".mpeg", ".mpg",  ".m4v",   ".3gp",
        ".m2ts", ".mts",    ".vob",      ".ogv",   ".rmvb", ".hevc", ".h264", ".h265",  ".flac",
        ".aac",  ".m4a",    ".ogg",      ".opus",  ".wma",  ".aiff", ".alac", ".ape",   ".mid",
        ".midi", ".so",     ".dylib",    ".a",     ".obj",  ".lib",  ".bin",  ".out",   ".elf",
        ".pdb",  ".deb",    ".rpm",      ".snap",  ".msi",  ".pkg",  ".dmg",  ".apk",   ".jar",
        ".war",  ".ear",    ".iso",      ".md5",   ".pdf",  ".gtk",  ".gtk1", ".gtk2",  ".gtk3",
        ".gtk4", ".gtk5",   ".gtk6"
    };

    if (_fs::is_directory(path)) {
        for (const _fs::path& item : _fs::recursive_directory_iterator(
               path, _fs::directory_options::skip_permission_denied)) {
            if (file_is_ignored(item)) {
                continue;
            }

            if (gitignore && repo.is_file_ignored(item)) {
                continue;
            }

            if (!_fs::is_regular_file(item)) {
                continue;
            }

            paths.push_back(item);
        }
    } else if (_fs::is_regular_file(path)) {
        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (!ft_to_skip.contains(ext) && !fima::fs::operations::is_file_executable(path) &&
            file_is_ignored(path)) {
            paths.push_back(path);
        }
    }

    return paths;
}

} // namespace fs

} // namespace fima
