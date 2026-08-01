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
#include <glob/glob.hpp>
#include <iostream>
#include <vector>

#include "theme.h"
#include "utility/regex.h"

void
remove(const std::vector<std::string>& paths_glob, const bool& recursive)
{
    for (const auto& entry : glob::rglob(paths_glob)) {
        try {
            if (!recursive && std::filesystem::is_directory(entry)) {
                std::cerr << fima::theme::theme.error << "Cannot remove directory "
                          << fima::theme::theme.secondary << entry.relative_path().string()
                          << fima::theme::theme.error << ". Becuase is not empty."
                          << fima::theme::Color::reset << '\n';

                continue;
            }

            if (std::filesystem::is_directory(entry)) {
                for (auto& item : std::filesystem::directory_iterator(
                       entry, std::filesystem::directory_options::skip_permission_denied)) {
                    std::cout << fima::theme::theme.primary << "Removed "
                              << (item.is_directory() ? "directory" : "file") << ": "
                              << fima::theme::theme.secondary
                              << std::filesystem::relative(item.path()).string()
                              << fima::theme::Color::reset << '\n';
                }
            }

            std::cout << fima::theme::theme.primary
                      << (std::filesystem::is_directory(entry) ? "Directory" : "File")
                      << " removed: " << fima::theme::theme.secondary << entry.filename().string()
                      << (std::filesystem::is_directory(entry) ? "/" : "")
                      << fima::theme::Color::reset << '\n';

            std::filesystem::remove_all(entry);
        } catch (const std::exception& ex) {
            std::cerr << fima::theme::theme.error
                      << "Failed to remove item: " << fima::theme::theme.secondary << entry.string()
                      << fima::theme::Color::reset << '\n';

            std::cerr << ex.what();
        }
    }
}

namespace fima {

namespace commands {

void
setup_remove(CLI::App& app, std::vector<std::string>& paths_glob, bool& recursive)
{
    CLI::App* subcmd =
      app.add_subcommand("rm", "Remove files and direcories (supports regex)")->configurable(false);

    subcmd->add_option("path", paths_glob, "File or directory to remove")
      ->configurable(false)
      ->required(true);

    subcmd
      ->add_flag("-r,--recursive", recursive, "Remove directories and their contents recursively")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->usage("fima remove [PATHS] [OPTIONS]");

    subcmd->callback([&]() { remove(paths_glob, recursive); });
}

} // namespace commands

} // namespace fima
