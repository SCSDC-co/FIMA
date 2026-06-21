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
#include "mappings.h"

namespace fima {

namespace fs {

File::File(const std::filesystem::directory_entry& path)
  : metadata(path)
  , file_type(fima::mappings::get_language_name(path.path()))
{
}

void
File::set_stats()
{
    this->stats = fima::cloc::helpers::count_lines(
      this->metadata.get_path(), fima::mappings::get_language_comments(this->metadata.get_path()));
    ;
}

[[nodiscard]] std::string
File::get_file_type() const
{
    return this->file_type;
}

} // namespace fs

} // namespace fima
