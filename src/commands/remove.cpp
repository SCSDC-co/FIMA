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
remove(const std::vector<std::regex>& paths, const bool& recursive)
{
    auto it = std::filesystem::recursive_directory_iterator(
      std::filesystem::current_path(), std::filesystem::directory_options::skip_permission_denied);

    for (const auto& entry : it) {
        if (fima::helpers::regex::matches_any_regex(entry.path().filename().string(), paths)) {
            try {
                if (recursive) {
                    std::filesystem::remove_all(entry);
                } else {
                    std::filesystem::remove(entry);
                }

                fima::logger::info(
                  true,
                  "remove",
                  fima::colors::GREEN + (entry.is_directory() ? "Directory" : "File") +
                    " removed: " + fima::colors::RESET + "{}",
                  entry.path().filename().string() + (entry.is_directory() ? "/" : ""));

                if (entry.is_directory()) {
                    it.disable_recursion_pending();
                    continue;
                }
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
