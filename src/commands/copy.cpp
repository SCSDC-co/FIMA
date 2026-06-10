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
#include <iostream>
#include <termcolor/termcolor.hpp>

void
copy_directory(std::filesystem::path source, std::filesystem::path destination)
{
    if (!std::filesystem::is_directory(source)) {
        std::cerr << termcolor::red
                  << "The source directory is a file or does not exists: " << termcolor::reset
                  << source.string() << '\n';
    }

    if (!std::filesystem::is_directory(destination)) {
        std::filesystem::create_directory(destination);
    }

    try {
        std::filesystem::copy(source,
                              destination,
                              std::filesystem::copy_options::overwrite_existing |
                                std::filesystem::copy_options::recursive);

        std::cout << source.string() << termcolor::green << " copied to " << termcolor::reset
                  << destination.string() << '\n';
    } catch (const std::exception& ex) {
        std::cerr << termcolor::red << "Failed to copy " << termcolor::reset << source.string()
                  << termcolor::red << " to " << termcolor::reset << destination.string() << '\n';

        std::cerr << ex.what();
    }
}

void
_copy_file(const std::filesystem::path source, const std::filesystem::path destination)
{
    if (!std::filesystem::is_regular_file(source)) {
        std::cerr << termcolor::red
                  << "The source file is a directory or does not exists: " << termcolor::reset
                  << source.string() << '\n';
    }

    try {
        std::filesystem::copy_file(
          source, destination, std::filesystem::copy_options::overwrite_existing);

        std::cout << source.string() << termcolor::green << " copied to " << termcolor::reset
                  << destination.string() << '\n';
    } catch (const std::exception& ex) {
        std::cerr << termcolor::red << "Failed to copy " << termcolor::reset << source.string()
                  << termcolor::red << " to " << termcolor::reset << destination.string() << '\n';

        std::cerr << ex.what();
    }
}

namespace fima {

namespace commands {

void
setup_copy(CLI::App& app, std::filesystem::path& source, std::filesystem::path& destination)
{
    CLI::App* subcmd = app.add_subcommand("cp", "Copy a file or a directory")->configurable(false);

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
