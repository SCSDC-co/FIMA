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

#include <nlohmann/json_fwd.hpp>

#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/language_file.h"
#include "commands/cloc/helpers/language_map.h"

namespace fima {

namespace fs {

File::File(const std::filesystem::directory_entry& path)
  : metadata(path)
{
    set_stats();
}

void
File::set_stats()
{
    using json = nlohmann::json;

    json languages_file = cloc::helpers::get_languages_file();

    std::string family = cloc::helpers::get_language_family(this->metadata.get_path());

    std::string single_comment = languages_file[family]["comments"]["single"].get<std::string>();
    std::string multiline_start =
      languages_file[family]["comments"]["multiline_start"].get<std::string>();
    std::string multiline_end =
      languages_file[family]["comments"]["multiline_end"].get<std::string>();

    this->stats = fima::cloc::helpers::count_lines(
      this->metadata.get_path(), single_comment, multiline_start, multiline_end);
}

} // namespace fs

} // namespace fima
