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

#include "fs/operations.h"
#include "fs/trash.h"
#include "options.h"
#include "theme.h"
#include "utility/regex.h"

void
remove(const fima::options::rm_options& options)
{
    for (const auto& entry : glob::rglob(options.paths)) {
        if (fima::fs::operations::is_root(entry)) {
            std::cerr << fima::theme::theme.error << "You cannot remove the root directory."
                      << fima::theme::Color::reset << '\n';

            continue;
        }

        if (fima::fs::operations::is_ancestor(entry, std::filesystem::current_path())) {
            std::cerr
              << fima::theme::theme.error
              << "You cannot remove a directory that contains the current working directory."
              << fima::theme::Color::reset << '\n';

            continue;
        }

        try {
            if (!options.recursive && std::filesystem::is_directory(entry) &&
                !std::filesystem::is_empty(entry)) {
                std::cerr << fima::theme::theme.error << "Cannot remove directory "
                          << fima::theme::theme.secondary << entry.relative_path().string()
                          << fima::theme::theme.error << ". Because is not empty."
                          << fima::theme::Color::reset << '\n';

                continue;
            }

            if (options.verbose) {
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
                          << " removed: " << fima::theme::theme.secondary
                          << entry.filename().string()
                          << (std::filesystem::is_directory(entry) ? "/" : "")
                          << fima::theme::Color::reset << '\n';
            }

            if (options.no_trash) {
                std::filesystem::remove_all(entry);
            } else {
                fima::fs::trash::add_file_to_trash(entry);
            }
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
setup_remove(CLI::App& app, fima::options::rm_options& options)
{
    CLI::App* subcmd =
      app.add_subcommand("rm", "Remove files and direcories (supports regex)")->configurable(false);

    subcmd->add_option("path", options.paths, "File or directory to remove")
      ->configurable(false)
      ->required(true);

    subcmd
      ->add_flag(
        "-r,--recursive", options.recursive, "Remove directories and their contents recursively")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-v,--verbose", options.verbose, "Outputs the names of the removed directory")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-t,--no-trash", options.no_trash, "Remove the item directly")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->usage("fima rm PATHS [OPTIONS]");

    subcmd->callback([&]() { remove(options); });
}

} // namespace commands

} // namespace fima
