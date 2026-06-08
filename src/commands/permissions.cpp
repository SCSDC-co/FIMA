/*
 * src/commands/permissions.cpp
 * include/commands/permissions.h
 *
 * The implementation of the `permissions` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/permissions.h"

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>
#include <vector>

#include "fs/operations.h"

namespace fima {

namespace perms {

void
print_perms(const std::string& permissions, const std::filesystem::path& entry)
{
    int i = 0;

    switch (permissions[0]) {
        case 'd':
            std::cout << termcolor::green;
            break;
        case 'l':
            std::cout << termcolor::blue;
            break;
        case '-':
            std::cout << termcolor::bold;
            break;
    }

    std::cout << permissions[0] << termcolor::reset;

    i++;

    for (; i < 4; ++i) {
        switch (permissions[i]) {
            case '-':
                std::cout << termcolor::bright_grey << '-' << termcolor::reset;
                break;
            case 'r':
                std::cout << termcolor::green << termcolor::bold << termcolor::underline << 'r'
                          << termcolor::reset;
                break;
            case 'w':
                std::cout << termcolor::yellow << termcolor::bold << termcolor::underline << 'w'
                          << termcolor::reset;
                break;
            case 'x':
                std::cout << termcolor::red << termcolor::bold << termcolor::underline << 'x'
                          << termcolor::reset;
                break;
        }
    }

    for (; i < permissions.length(); ++i) {
        switch (permissions[i]) {
            case '-':
                std::cout << termcolor::bright_grey << '-' << termcolor::reset;
                break;
            case 'r':
                std::cout << termcolor::green << 'r' << termcolor::reset;
                break;
            case 'w':
                std::cout << termcolor::yellow << 'w' << termcolor::reset;
                break;
            case 'x':
                std::cout << termcolor::red << 'x' << termcolor::reset;
                break;
        }
    }

    std::cout << "  ";

    if (std::filesystem::is_directory(entry)) {
        std::cout << termcolor::green;
    } else if (fima::fs::operations::is_file_executable(entry)) {
        std::cout << termcolor::red;
    }

    std::cout << entry.string() << (std::filesystem::is_directory(entry) ? "/" : "")
              << termcolor::reset << '\n';
}

} // namespace perms

namespace commands {

void
permissions(const std::vector<std::filesystem::path>& paths)
{
    for (const std::filesystem::path& item : paths) {
        if (!std::filesystem::exists(item)) {
            std::cerr << termcolor::red << "The path doesn't exists: " << termcolor::reset
                      << item.string() << '\n';

            continue;
        }

        std::string perms = fima::fs::operations::get_perms(item);

        fima::perms::print_perms(perms, item);
    }
}

void
setup_permissions(CLI::App& app, std::vector<std::filesystem::path>& paths)
{
    CLI::App* subcmd =
      app.add_subcommand("perms", "Show a directory/file permissions")->configurable(false);

    subcmd->add_option("path", paths, "The file path to read permissions from")
      ->configurable(false)
      ->required(true);

    subcmd->usage("fima perms [PATHS]");

    subcmd->callback([&]() { permissions(paths); });
}

} // namespace commands

} // namespace fima
