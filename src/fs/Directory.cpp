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
#include "config.h"
#include "fs/get_directories_entries.h"
#include "git/GitRepo.h"
#include "program_files.h"

namespace fima {

namespace fs {

Directory::Directory(const std::filesystem::directory_entry& path)
  : metadata(path)
{
}

void
Directory::set_stats(const fima::git::GitRepo& repo)
{
    fima::cloc::classes::Stats stats;

    std::vector<std::filesystem::path> entries{ fima::fs::get_directories_for_cloc(
      this->metadata.get_path(), repo, true, true, fima::config::DEFAULT_DIRS_TO_IGNORE) };

    for (const std::filesystem::path& item : entries) {
        if (!std::filesystem::is_directory(item)) {
            std::string family = fima::program_files::get_language_family(item);

            std::string single_comment =
              fima::program_files::language_file_json[family]["comments"]["single"]
                .get<std::string>();
            std::string multiline_start =
              fima::program_files::language_file_json[family]["comments"]["multiline_start"]
                .get<std::string>();
            std::string multiline_end =
              fima::program_files::language_file_json[family]["comments"]["multiline_end"]
                .get<std::string>();

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

    std::vector<std::filesystem::path> entries{ fima::fs::get_directories_entries_recursive(
      this->metadata.get_path(), { "." }, true, {}) };

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
