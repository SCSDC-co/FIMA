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

#include "utility/regex.h"

namespace _fs = std::filesystem;

namespace fima {

namespace fs {

std::vector<_fs::directory_entry>
get_directories_entries(const _fs::path& path, const bool& dotfiles)
{
    std::vector<_fs::directory_entry> directory_content;

    for (const _fs::directory_entry& entry : _fs::directory_iterator(path)) {
        std::string name{ entry.path().filename().string() };

        if (name[0] == '.' && !dotfiles) {
            continue;
        }

        directory_content.push_back(entry);
    }

    return directory_content;
}

std::vector<_fs::path>
get_directories_entries_recursive(const _fs::path& path,
                                  const bool& ignore_directories,
                                  const std::vector<std::regex>& ignored_files_directories)
{
    std::vector<_fs::path> directory_content;

    for (const auto& entry : _fs::directory_iterator(path)) {
        std::string name = entry.path().filename().string();

        if (ignore_directories &&
            fima::helpers::regex::matches_any_regex(name, ignored_files_directories)) {
            continue;
        }

        directory_content.push_back(entry.path());

        if (entry.is_directory()) {
            std::vector<_fs::path> sub_entries = get_directories_entries_recursive(
              entry.path(), ignore_directories, ignored_files_directories);

            directory_content.insert(
              directory_content.end(), sub_entries.begin(), sub_entries.end());
        }
    }

    return directory_content;
}

} // namespace fs

} // namespace fima
