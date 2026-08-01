/*
 * src/commands/rename.cpp
 * include/commands/rename.h
 *
 * The implementation of the `rename` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/rename.h"

#include <filesystem>
#include <iostream>
#include <string>

#include "theme.h"

namespace fs = std::filesystem;

void
_rename(const fs::path old_name, const fs::path new_name)
{
    if (!fs::exists(old_name)) {
        std::cerr << fima::theme::theme.error
                  << "The item you want to rename does not exist: " << fima::theme::theme.secondary
                  << old_name.string() << fima::theme::Color::reset << '\n';

        return;
    }

    if (fs::exists(new_name)) {
        std::cerr << fima::theme::theme.error
                  << "The item already exists: " << fima::theme::theme.secondary
                  << new_name.string() << fima::theme::Color::reset << '\n';

        return;
    }

    try {
        fs::rename(old_name, new_name);

        std::cout << old_name.string() << fima::theme::theme.primary
                  << " renamed to: " << fima::theme::theme.secondary << new_name.string()
                  << fima::theme::Color::reset << '\n';
    } catch (const std::exception& ex) {
        std::cerr << fima::theme::theme.error
                  << "Failed to rename: " << fima::theme::theme.secondary << old_name.string()
                  << fima::theme::theme.error << " to " << fima::theme::theme.secondary
                  << new_name.string() << fima::theme::Color::reset << '\n';

        std::cerr << ex.what();
    }
}

namespace fima {

namespace commands {

void
setup_rename(CLI::App& app, fs::path& old_name, fs::path& new_name)
{
    CLI::App* subcmd =
      app.add_subcommand("mv", "Move/rename a file or a directory")->configurable(false);

    subcmd->add_option("old-name", old_name, "File or directory to move or rename")
      ->configurable(false)
      ->required(true);
    subcmd->add_option("new-name", new_name, "The new name for the directory or file")
      ->configurable(false)
      ->required(true);

    subcmd->usage("fima rename [OLD NAME] [NEW NAME]");

    subcmd->callback([&]() { _rename(old_name, new_name); });
}

} // namespace commands

} // namespace fima
