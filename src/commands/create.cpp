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
#include <termcolor/termcolor.hpp>
#include <vector>

#include "fs/operations.h"

void
file(const std::vector<std::filesystem::path>& paths)
{
    for (const auto& entry : paths) {
        if (std::filesystem::exists(entry)) {
            std::cerr << termcolor::red << "This file already exists: " << termcolor::reset
                      << entry.string() << '\n';

            continue;
        }

        try {
            fima::fs::operations::create(entry, "");

            std::cout << termcolor::green << "File created at: " << termcolor::reset
                      << entry.string() << '\n';
        } catch (const std::exception& ex) {
            std::cerr << termcolor::red << "Failed to create the file: " << termcolor::reset
                      << entry.string() << '\n';

            std::cerr << ex.what();
        }
    }
}

void
dir(const std::vector<std::filesystem::path>& paths)
{
    for (const auto& entry : paths) {
        if (std::filesystem::exists(entry)) {
            std::cerr << termcolor::red << "This directory already exists: " << termcolor::reset
                      << entry.string() << '\n';

            continue;
        }

        try {
            std::filesystem::create_directories(entry);

            std::cout << termcolor::green << "Directory created at: " << termcolor::reset
                      << entry.string() << '\n';
        } catch (const std::exception& ex) {
            std::cerr << termcolor::red << "Failed to create the directory: " << termcolor::reset
                      << entry.string() << '\n';

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
      app.add_subcommand("create", "Create a directory or a file")->configurable(false);

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
