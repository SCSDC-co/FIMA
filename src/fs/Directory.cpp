/*
 * src/fs/File.cpp
 * include/fs/File.h
 *
 * A class that represent a directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/Directory.h"

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <vector>

#include "commands/cloc/helpers/Stats.h"
#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/language_file.h"
#include "commands/cloc/helpers/language_map.h"
#include "config.h"
#include "helpers/get_directories_entries.h"

namespace fima {

namespace fs {

Directory::Directory(const std::filesystem::directory_entry& path)
  : metadata(path)
{
    this->set_stats();
    this->set_number_of_files();
}

void
Directory::set_stats()
{
    using json = nlohmann::json;

    fima::cloc::classes::Stats stats;

    json languages_file = cloc::helpers::get_languages_file();

    std::vector<std::filesystem::path> entries{ fima::helpers::get_directories_entries_recursive(
      this->metadata.get_path(), true, fima::config::DEFAULT_DIRS_TO_IGNORE) };

    for (const std::filesystem::path& item : entries) {
        if (!std::filesystem::is_directory(item)) {
            std::string family = cloc::helpers::get_language_family(item);

            std::string single_comment =
              languages_file[family]["comments"]["single"].get<std::string>();
            std::string multiline_start =
              languages_file[family]["comments"]["multiline_start"].get<std::string>();
            std::string multiline_end =
              languages_file[family]["comments"]["multiline_end"].get<std::string>();

            stats += fima::cloc::helpers::count_lines(
              item, single_comment, multiline_start, multiline_end);
        }
    }

    this->stats = stats;
}

void
Directory::set_number_of_files()
{
    int number{ 0 };

    std::vector<std::filesystem::path> entries{ fima::helpers::get_directories_entries_recursive(
      this->metadata.get_path(), true, {}) };

    for (const std::filesystem::path& item : entries) {
        if (!std::filesystem::is_directory(item)) {
            ++number;
        }
    }

    this->number_of_files = number;
}

[[nodiscard]] int
Directory::get_number_of_files() const
{
    return this->number_of_files;
}

} // namespace fs

} // namespace fima
