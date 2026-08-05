/*
 * src/commands/trash/empty.cpp
 * include/commands/trash/empty.h
 *
 * The declaration of the `trash empty` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/trash/empty.h"

#include <filesystem>
#include <iostream>

#include "fs/operations.h"
#include "fs/trash.h"
#include "theme.h"

namespace fima {

namespace commands {

namespace trash {

void
empty(const bool& yes)
{
    int i{ 0 };

    std::size_t size{};

    for (auto& it : std::filesystem::directory_iterator(
           fima::fs::trash::TRASH_FILES_PATH,
           std::filesystem::directory_options::skip_permission_denied)) {
        if (it.is_directory()) {
            for (auto& _it : std::filesystem::recursive_directory_iterator(
                   it, std::filesystem::directory_options::skip_permission_denied)) {
                if (_it.is_directory()) {
                    continue;
                }

                size += _it.file_size();
            }
        } else {
            size += it.file_size();
        }

        ++i;
    }

    if (i == 0) {
        std::cout << fima::theme::theme.primary << "The trash is already empty."
                  << fima::theme::Color::reset << '\n';

        return;
    }

    if (yes) {
        std::filesystem::remove_all(fima::fs::trash::TRASH_FILES_PATH);
        std::filesystem::remove_all(fima::fs::trash::TRASH_TRASHINFO_PATH);

        std::filesystem::create_directories(fima::fs::trash::TRASH_FILES_PATH);
        std::filesystem::create_directories(fima::fs::trash::TRASH_TRASHINFO_PATH);

        std::cout << fima::theme::theme.primary << "Trash emptied successfully!"
                  << fima::theme::Color::reset << '\n';

        return;
    }

    std::string size_str{ fima::fs::operations::make_size_readable(size) };

    if (size_str.back() == ' ') {
        size_str.pop_back();
    }

    char confirm{};

    std::cout << fima::theme::theme.primary << "The trash contains " << i << " items (" << size_str
              << ")." << '\n';
    std::cout << "Are you sure you want to permanently delete them? [y/N]: "
              << fima::theme::Color::reset;

    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        std::filesystem::remove_all(fima::fs::trash::TRASH_FILES_PATH);
        std::filesystem::remove_all(fima::fs::trash::TRASH_TRASHINFO_PATH);

        std::filesystem::create_directories(fima::fs::trash::TRASH_FILES_PATH);
        std::filesystem::create_directories(fima::fs::trash::TRASH_TRASHINFO_PATH);

        std::cout << '\n'
                  << fima::theme::theme.primary << "Trash emptied successfully!"
                  << fima::theme::Color::reset << '\n';
    }
}

} // namespace trash

} // namespace commands

} // namespace fima
