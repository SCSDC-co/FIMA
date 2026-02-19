/*
 * src/helpers/get_directories_entries.cpp
 * include/helpers/get_directories_entries.h
 *
 * This file contains the helpers for getting the entries of a directory, both recursively and not
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "helpers/get_directories_entries.h"

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

#include "utility/regex.h"

namespace fs = std::filesystem;

namespace fima {

namespace helpers {

std::vector<fs::directory_entry>
get_directories_entries(const fs::path& path, const bool& dotfiles)
{
    std::vector<fs::directory_entry> directory_content;

    for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
        std::string name{ entry.path().filename().string() };

        if (name[0] == '.' && !dotfiles) {
            continue;
        }

        directory_content.push_back(entry);
    }

    return directory_content;
}

std::vector<fs::path>
get_directories_entries_recursive(const fs::path& path,
                                  const bool& ignore_directories,
                                  const std::vector<std::regex>& ignored_files_directories)
{
    std::vector<fs::path> directory_content;

    for (const auto& entry : fs::directory_iterator(path)) {
        std::string name = entry.path().filename().string();

        if (ignore_directories &&
            fima::helpers::regex::matches_any_regex(name, ignored_files_directories)) {
            continue;
        }

        directory_content.push_back(entry.path());

        if (entry.is_directory()) {
            std::vector<fs::path> sub_entries = get_directories_entries_recursive(
              entry.path(), ignore_directories, ignored_files_directories);

            directory_content.insert(
              directory_content.end(), sub_entries.begin(), sub_entries.end());
        }
    }

    return directory_content;
}

} // namespace helpers

} // namespace fima
