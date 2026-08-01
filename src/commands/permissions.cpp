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
#include <rang.hpp>
#include <string>
#include <vector>

#include "fs/operations.h"
#include "theme.h"

namespace fima {

namespace perms {

void
print_perms(const std::string& permissions, const std::filesystem::path& entry)
{
    int i = 0;

    switch (permissions[0]) {
        case 'd':
            std::cout << fima::theme::theme.directory;
            break;
        case 'l':
            std::cout << fima::theme::theme.symlink;
            break;
        case '-':
            std::cout << rang::style::bold;
            break;
    }

    std::cout << permissions[0] << theme::Color::reset;

    i++;

    for (; i < 4; ++i) {
        switch (permissions[i]) {
            case '-':
                std::cout << fima::theme::theme.perms_null << '-' << fima::theme::Color::reset;
                break;
            case 'r':
                std::cout << fima::theme::theme.perms_read << rang::style::bold
                          << rang::style::underline << 'r' << fima::theme::Color::reset;
                break;
            case 'w':
                std::cout << fima::theme::theme.perms_write << rang::style::bold
                          << rang::style::underline << 'w' << fima::theme::Color::reset;
                break;
            case 'x':
                std::cout << fima::theme::theme.perms_exec << rang::style::bold
                          << rang::style::underline << 'x' << fima::theme::Color::reset;
                break;
        }
    }

    for (; i < permissions.length(); ++i) {
        switch (permissions[i]) {
            case '-':
                std::cout << fima::theme::theme.perms_null << '-' << fima::theme::Color::reset;
                break;
            case 'r':
                std::cout << fima::theme::theme.perms_read << 'r' << fima::theme::Color::reset;
                break;
            case 'w':
                std::cout << fima::theme::theme.perms_write << 'w' << fima::theme::Color::reset;
                break;
            case 'x':
                std::cout << fima::theme::theme.perms_exec << 'x' << fima::theme::Color::reset;
                break;
        }
    }

    std::cout << "  ";

    if (std::filesystem::is_directory(entry)) {
        std::cout << fima::theme::theme.directory;
    } else if (fima::fs::operations::is_file_executable(entry)) {
        std::cout << fima::theme::theme.executable;
    }

    std::cout << entry.string() << (std::filesystem::is_directory(entry) ? "/" : "")
              << fima::theme::Color::reset << '\n';
}

} // namespace perms

namespace commands {

void
permissions(const std::vector<std::filesystem::path>& paths)
{
    for (const std::filesystem::path& item : paths) {
        if (!std::filesystem::exists(item)) {
            std::cerr << fima::theme::theme.error
                      << "The path doesn't exists: " << fima::theme::Color::reset << item.string()
                      << '\n';

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
