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
#include <iostream>
#include <regex>
#include <termcolor/termcolor.hpp>
#include <vector>

#include "utility/regex.h"

void
remove(const std::vector<std::regex>& paths, const bool& recursive)
{
    auto it = std::filesystem::directory_iterator(
      std::filesystem::current_path(), std::filesystem::directory_options::skip_permission_denied);

    for (const auto& entry : it) {
        if (fima::utility::regex::matches_any_regex(entry.path().filename().string(), paths)) {
            try {
                if (!recursive && entry.is_directory()) {
                    std::cerr << termcolor::red << "Cannot remove directory " << termcolor::reset
                              << entry.path().relative_path().string() << termcolor::red
                              << ". Becuase is not empty." << '\n';

                    continue;
                }

                if (entry.is_directory()) {
                    for (auto& item : std::filesystem::directory_iterator(
                           entry, std::filesystem::directory_options::skip_permission_denied)) {
                        std::cout << termcolor::green << "Removed "
                                  << (item.is_directory() ? "directory" : "file") << ": "
                                  << termcolor::reset
                                  << std::filesystem::relative(item.path()).string() << '\n';
                    }
                }

                std::cout << termcolor::green << (entry.is_directory() ? "Directory" : "File")
                          << " removed: " << termcolor::reset << entry.path().filename().string()
                          << (entry.is_directory() ? "/" : "") << '\n';

                std::filesystem::remove_all(entry.path());
            } catch (const std::exception& ex) {
                std::cerr << termcolor::red << "Failed to remove item: " << termcolor::reset
                          << entry.path().string() << '\n';

                std::cerr << ex.what();
            }
        }
    }
}

namespace fima {

namespace commands {

void
setup_remove(CLI::App& app, std::vector<std::filesystem::path>& paths, bool& recursive)
{
    CLI::App* subcmd =
      app.add_subcommand("rm", "Remove files and direcories (supports regex)")->configurable(false);

    subcmd->add_option("path", paths, "File or directory to remove")
      ->configurable(false)
      ->required(true);

    subcmd
      ->add_flag("-r,--recursive", recursive, "Remove directories and their contents recursively")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->usage("fima remove [PATHS] [OPTIONS]");

    subcmd->callback([&]() {
        std::vector<std::regex> regexes;

        for (const std::filesystem::path& path : paths) {
            regexes.push_back(fima::utility::regex::glob_to_regex(path.string()));
        }

        remove(regexes, recursive);
    });
}

} // namespace commands

} // namespace fima
