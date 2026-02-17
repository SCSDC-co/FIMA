/*
 * src/commands/ls.cpp
 * include/commands/ls.h
 *
 * The implementation of the `ls` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/ls.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

#include "helpers/get_directories_entries.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

void
start(const fs::path& path)
{
    std::vector<fs::directory_entry> list_of_the_directory{ fima::helpers::get_directories_entries(
      path) };

    std::sort(list_of_the_directory.begin(),
              list_of_the_directory.end(),
              [](const fs::directory_entry& a, const fs::directory_entry& b) {
                  return a.is_directory() && !b.is_directory();
              });

    for (const fs::directory_entry& item : list_of_the_directory) {
        if (item.is_directory()) {
            std::cout << fima::colors::GREEN << fima::colors::BOLD;
        }

        std::cout << item.path().filename().string();

        if (item.is_directory()) {
            std::cout << "/";
        }

        std::cout << fima::colors::RESET << "  ";
    }

    std::cout << '\n';
}

} // namespace ls

} // namespace fima
