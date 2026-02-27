/*
 * src/commands/remove.cpp
 * include/commands/remove.h
 *
 * The implementation of the `remove` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/remove.h"

#include <filesystem>
#include <regex>
#include <vector>

#include "logger.h"
#include "utility/colors.h"
#include "utility/regex.h"

namespace fima {

void
remove(const std::vector<std::regex>& paths)
{
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(std::filesystem::current_path())) {
        if (fima::helpers::regex::matches_any_regex(entry.path().filename().string(), paths)) {
            try {
                std::filesystem::remove_all(entry);

                fima::logger::info(true,
                                   "remove",
                                   fima::colors::GREEN + "Item removed: " + fima::colors::RESET +
                                     "{}",
                                   entry.path().filename().string());
            } catch (const std::exception& ex) {
                fima::logger::error(true,
                                    "remove",
                                    fima::colors::RED +
                                      "Failed to remove item: " + fima::colors::RESET + "{}",
                                    entry.path().string());
                fima::logger::error(true, "remove", ex.what());
            }
        }
    }
}

} // namespace fima
