/*
 * src/commands/create.cpp
 * include/commands/create.h
 *
 * The implementation of the `create` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/create.h"

#include <filesystem>
#include <iostream>
#include <vector>

#include "fs/operations.h"
#include "theme.h"

void
file(const std::vector<std::filesystem::path>& paths)
{
    for (const auto& entry : paths) {
        if (std::filesystem::exists(entry)) {
            std::cerr << fima::theme::theme.error
                      << "This file already exists: " << fima::theme::theme.secondary
                      << entry.string() << fima::theme::Color::reset << '\n';

            continue;
        }

        try {
            fima::fs::operations::create(entry, "");

            std::cout << fima::theme::theme.primary
                      << "File created at: " << fima::theme::theme.secondary << entry.string()
                      << fima::theme::Color::reset << '\n';
        } catch (const std::exception& ex) {
            std::cerr << fima::theme::theme.error
                      << "Failed to create the file: " << fima::theme::theme.secondary
                      << entry.string() << fima::theme::Color::reset << '\n';

            std::cerr << ex.what();
        }
    }
}

void
dir(const std::vector<std::filesystem::path>& paths)
{
    for (const auto& entry : paths) {
        if (std::filesystem::exists(entry)) {
            std::cerr << fima::theme::theme.error
                      << "This directory already exists: " << fima::theme::theme.secondary
                      << entry.string() << fima::theme::Color::reset << '\n';

            continue;
        }

        try {
            std::filesystem::create_directories(entry);

            std::cout << fima::theme::theme.primary
                      << "Directory created at: " << fima::theme::theme.secondary << entry.string()
                      << fima::theme::Color::reset << '\n';
        } catch (const std::exception& ex) {
            std::cerr << fima::theme::theme.error
                      << "Failed to create the directory: " << fima::theme::theme.secondary
                      << entry.string() << fima::theme::Color::reset << '\n';

            std::cerr << ex.what();
        }
    }
}

namespace fima {

namespace commands {

void
setup_create(CLI::App& app,
             std::vector<std::filesystem::path>& file_paths,
             std::vector<std::filesystem::path>& dir_paths)
{
    CLI::App* subcmd =
      app.add_subcommand("mk", "Create files and/or directories")->configurable(false);

    subcmd->add_option("-f,--file", file_paths, "Files to create")->configurable(false);

    subcmd->add_option("-d,--dir", dir_paths, "Directories to create")->configurable(false);

    subcmd->usage("fima create -d [DIRECTORIES] -f [FILES]");

    subcmd->callback([&]() {
        dir(dir_paths);
        file(file_paths);
    });
}

} // namespace commands

} // namespace fima
