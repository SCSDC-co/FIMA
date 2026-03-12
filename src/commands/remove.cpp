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

namespace fima {

namespace commands {

void
setup_remove(CLI::App& app, std::vector<std::filesystem::path>& paths)
{
    CLI::App* subcmd = app.add_subcommand("remove", "Remove files and direcories (supports regex)")
                         ->configurable(false);

    subcmd->add_option("path", paths, "File or directory to remove")
      ->configurable(false)
      ->required(true);

    bool remove_subcmd_recursive{ false };
    subcmd
      ->add_flag("-r,--recursive",
                 remove_subcmd_recursive,
                 "Remove directories and their contents recursively")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->usage("fima remove [PATHS] [OPTIONS]");

    subcmd->callback([=]() {
        std::vector<std::regex> regexes;

        for (const std::filesystem::path& path : paths) {
            regexes.push_back(fima::helpers::regex::glob_to_regex(path.filename().string()));
        }

        remove(regexes, remove_subcmd_recursive);
    });
}

} // namespace commands

} // namespace fima
