/*
 * src/commands/ls/ls.cpp
 * include/commands/ls/ls.h
 *
 * The implementation of the `ls` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/ls/ls.h"

#include <algorithm>
#include <filesystem>
#include <vector>

#include "commands/ls/helpers/printer.h"
#include "helpers/get_directories_entries.h"
#include "options.h"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

void
start(const fs::path& path, const fima::options::ls_options& options)
{
    std::vector<fs::directory_entry> list_of_the_directory{ fima::helpers::get_directories_entries(
      path, options.all) };

    // what was I thinking when I did the 2 vector thing? Sorting the vector is much better
    std::sort(list_of_the_directory.begin(),
              list_of_the_directory.end(),
              [](const fs::directory_entry& a, const fs::directory_entry& b) {
                  return a.is_directory() && !b.is_directory();
              });

    std::vector<fs::path> entries{};

    for (const fs::directory_entry& item : list_of_the_directory) {
        entries.push_back(item.path().filename());
    }

    helpers::print(entries, options.icons);
}

} // namespace ls

} // namespace fima
