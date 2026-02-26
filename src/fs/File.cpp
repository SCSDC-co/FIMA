/*
 * src/fs/File.cpp
 * include/fs/File.h
 *
 * A class that represent a file
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/File.h"

#include <filesystem>
#include <nlohmann/json_fwd.hpp>

#include "commands/cloc/helpers/count_lines.h"
#include "program_files.h"

namespace fima {

namespace fs {

File::File(const std::filesystem::directory_entry& path)
  : metadata(path)
  , file_type(fima::program_files::get_language_name(path.path()))
{
}

void
File::set_stats()
{
    std::string family = fima::program_files::get_language_family(this->metadata.get_path());

    std::string single_comment =
      fima::program_files::language_file_json[family]["comments"]["single"].get<std::string>();
    std::string multiline_start =
      fima::program_files::language_file_json[family]["comments"]["multiline_start"]
        .get<std::string>();
    std::string multiline_end =
      fima::program_files::language_file_json[family]["comments"]["multiline_end"]
        .get<std::string>();

    this->stats = fima::cloc::helpers::count_lines(
      this->metadata.get_path(), single_comment, multiline_start, multiline_end);
}

[[nodiscard]] std::string
File::get_file_type() const
{
    return this->file_type;
}

} // namespace fs

} // namespace fima
