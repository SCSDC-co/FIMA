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
#include <vector>

#include "fs/FileDetector.h"
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
            fima::utility::regex::matches_any_regex(name, ignored_files_or_directories)) {
            it.disable_recursion_pending();
            continue;
        }

        directory_content.push_back(entry.path());
    }

    return directory_content;
}

std::vector<_fs::directory_entry>
get_files_for_cloc(const _fs::directory_entry& path,
                   const fima::git::GitRepo& repo,
                   const bool& gitignore,
                   const bool& ignore,
                   const std::vector<std::regex>& ignored_files_or_directories)
{
    std::vector<_fs::directory_entry> paths{};

    auto file_is_ignored = [&](const std::filesystem::path& path) {
        return ignore && fima::utility::regex::matches_any_regex(path.filename().string(),
                                                                 ignored_files_or_directories);
    };

    auto is_file_valid = [&](const std::filesystem::path& path) {
        std::string ext{ path.extension().string() };
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        return ext != ".md5" && fima::fs::file_detector.is_file_clocable(path);
    };

    if (_fs::is_directory(path)) {
        auto it =
          _fs::recursive_directory_iterator(path, _fs::directory_options::skip_permission_denied);

        for (const _fs::directory_entry& item : it) {
            if (file_is_ignored(item)) {
                it.disable_recursion_pending();
                continue;
            }

            if (gitignore && repo.is_file_ignored(item)) {
                it.disable_recursion_pending();
                continue;
            }

            if (is_file_valid(item)) {
                paths.push_back(item);
            }
        }
    } else if (_fs::is_regular_file(path)) {
        if (is_file_valid(path)) {
            paths.push_back(path);
        }
    }

    return paths;
}

} // namespace fs

} // namespace fima
