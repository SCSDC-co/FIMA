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
#include <vector>

#include "fs/operations.h"
#include "logger.h"

void
file(const std::vector<std::filesystem::path>& paths)
{
    for (const auto& entry : paths) {
        if (std::filesystem::exists(entry)) {
            fima::logger::error(true,
                                "create file",
                                fima::colors::RED +
                                  "This item already exists: " + fima::colors::RESET + "{}",
                                entry.string());

            continue;
        }

        try {
            fima::fs::operations::create(entry, "");

            fima::logger::info(true,
                               "create file",
                               fima::colors::GREEN + "File created at: " + fima::colors::RESET +
                                 "{}",
                               entry.string());
        } catch (const std::exception& ex) {
            fima::logger::error(true,
                                "create file",
                                fima::colors::RED +
                                  "Failed to create the file: " + fima::colors::RESET + "{}",
                                entry.string());
            fima::logger::error(true, "create file", ex.what());
        }
    }
}

void
dir(const std::vector<std::filesystem::path>& paths)
{
    for (const auto& entry : paths) {
        if (std::filesystem::exists(entry)) {
            fima::logger::error(true,
                                "create dir",
                                fima::colors::RED +
                                  "This item already exists: " + fima::colors::RESET + "{}",
                                entry.string());

            continue;
        }

        try {
            std::filesystem::create_directories(entry);

            fima::logger::info(true,
                               "create dir",
                               fima::colors::GREEN +
                                 "Directory created at: " + fima::colors::RESET + "{}",
                               entry.string());
        } catch (const std::exception& ex) {
            fima::logger::error(true,
                                "create dir",
                                fima::colors::RED +
                                  "Failed to create the directory: " + fima::colors::RESET + "{}",
                                entry.string());
            fima::logger::error(true, "create dir", ex.what());
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
