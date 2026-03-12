/*
 * src/commands/copy.cpp
 * include/commands/copy.h
 *
 * The implementation of the `copy` subcommand when copying files
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/copy.h"

#include <filesystem>

#include "cli/CLI11.hpp"
#include "logger.h"
#include "utility/colors.h"

void
copy_directory(std::filesystem::path source, std::filesystem::path destination)
{
    if (!std::filesystem::is_directory(source)) {
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "The source file is a file or does not exists: " +
                              fima::colors::RESET + "{}",
                            source.string());
    }

    if (!std::filesystem::is_directory(destination)) {
        std::filesystem::create_directory(destination);
    }

    try {
        std::filesystem::copy(source,
                              destination,
                              std::filesystem::copy_options::overwrite_existing |
                                std::filesystem::copy_options::recursive);

        fima::logger::info(true,
                           "copy dir",
                           fima::colors::GREEN + "Directory " + fima::colors::RESET + "{}" +
                             fima::colors::GREEN + " copied to " + fima::colors::RESET + "{}",
                           source.string(),
                           destination.string());
    } catch (const std::exception& ex) {
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "Failed to copy the directory" +
                              fima::colors::RESET);
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "  Source directory: " + fima::colors::RESET + "{}",
                            source.string());
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "  Destination: " + fima::colors::RESET + "{}",
                            destination.string());

        fima::logger::error(true, "copy dir", ex.what());
    }
}

void
_copy_file(const std::filesystem::path source, const std::filesystem::path destination)
{
    if (!std::filesystem::is_regular_file(source)) {
        fima::logger::error(
          true,
          "copy file",
          fima::colors::RED +
            "The source file is a directory or does not exists: " + fima::colors::RESET + "{}",
          source.string());
    }

    try {
        std::filesystem::copy_file(
          source, destination, std::filesystem::copy_options::overwrite_existing);

        fima::logger::info(true,
                           "copy file",
                           fima::colors::GREEN + "File " + fima::colors::RESET + "{}" +
                             fima::colors::GREEN + " copied to " + fima::colors::RESET + "{}",
                           source.string(),
                           destination.string());
    } catch (const std::exception& ex) {
        fima::logger::error(
          true, "copy file", fima::colors::RED + "Failed to copy the file" + fima::colors::RESET);
        fima::logger::error(true,
                            "copy file",
                            fima::colors::RED + "  Source directory: " + fima::colors::RESET + "{}",
                            source.string());
        fima::logger::error(true,
                            "copy file",
                            fima::colors::RED + "  Destination: " + fima::colors::RESET + "{}",
                            destination.string());

        fima::logger::error(true, "copy file", ex.what());
    }
}

namespace fima {

namespace commands {

void
setup_copy(CLI::App& app, std::filesystem::path& source, std::filesystem::path& destination)
{
    CLI::App* subcmd =
      app.add_subcommand("copy", "Copy a file or a directory")->configurable(false);

    subcmd->add_option("source-file", source, "File or directory copy")
      ->configurable(false)
      ->required(true);
    subcmd->add_option("destination", destination, "Destination")
      ->configurable(false)
      ->required(true);

    subcmd->usage("fima copy [SOURCE] [DESTINATION]");

    subcmd->callback([&]() {
        if (std::filesystem::is_regular_file(source)) {
            _copy_file(source, destination);
        } else if (std::filesystem::is_directory(source)) {
            copy_directory(source, destination);
        }
    });
}

} // namespace commands

} // namespace fima
