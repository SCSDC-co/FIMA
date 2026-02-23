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
#include "logger.h"
#include "options.h"
#include "utility/DirectoryItem.h"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

void
start(const fs::path& path, const fima::options::ls_options& options)
{
    std::vector<fs::directory_entry> list_of_the_directory{ fima::helpers::get_directories_entries(
      path, options.all) };

    std::sort(list_of_the_directory.begin(),
              list_of_the_directory.end(),
              [](const fs::directory_entry& a, const fs::directory_entry& b) {
                  return a.path().filename().string() < b.path().filename().string();
              });

    // what was I thinking when I did the 2 vector thing? Sorting the vector is much better
    std::sort(list_of_the_directory.begin(),
              list_of_the_directory.end(),
              [](const fs::directory_entry& a, const fs::directory_entry& b) {
                  return a.is_directory() && !b.is_directory();
              });

    std::vector<fima::DirectoryItem> entries{};

    for (const fs::directory_entry& item : list_of_the_directory) {
        fima::DirectoryItem file{ item.path() };

        entries.push_back(file);
    }

    if (options.long_output) {
        helpers::print_long(entries, options.icons, options.verbose);
    } else {
        helpers::print_normal(entries, options.icons);
    }

    fima::logger::info(false, "ls", "Got list of directory: {}", path.string());
    fima::logger::info(false, "ls", "Options:");
    fima::logger::info(false, "ls", "  All: {}", (options.all ? "true" : "false"));
    fima::logger::info(false, "ls", "  Icons: {}", (options.icons ? "true" : "false"));
    fima::logger::info(false, "ls", "  Long output: {}", (options.long_output ? "true" : "false"));
    fima::logger::info(false, "ls", "  Verbose: {}", (options.verbose ? "true" : "false"));
}

} // namespace ls

} // namespace fima
