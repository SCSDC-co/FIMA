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
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "helpers/logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

void
get_perms(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {
        if (!fs::exists(entry)) {
            fima::helpers::log(
              fima::helpers::logger_type::ERROR, "The path doesn't exists: ", entry.string());

            continue;
        }

        try {
            auto st = fs::status(entry);

            fs::perms permissions{ st.permissions() };

            auto show = [=](const std::string& color, char op, fs::perms perm) {
                std::cout << color << (fs::perms::none == (perm & permissions) ? '-' : op)
                          << fima::colors::RESET;
            };

            auto print_ft = [](const std::string_view& color, char c) {
                std::cout << color << fima::colors::BOLD << c << fima::colors::RESET;
            };

            if (fs::is_regular_file(st)) {
                print_ft("", '-');
            } else if (fs::is_directory(st)) {
                print_ft(fima::colors::GREEN, 'd');
            } else if (fs::is_symlink(st)) {
                print_ft(fima::colors::BLUE, 'l');
            } else if (fs::is_character_file(st)) {
                print_ft("", 'c');
            } else if (fs::is_block_file(st)) {
                print_ft("", 'b');
            } else if (fs::is_fifo(st)) {
                print_ft("", 'p');
            } else if (fs::is_socket(st)) {
                print_ft("", 's');
            } else {
                print_ft("", '?'); // unknown type
            }

            show(std::string(fima::colors::RED) + std::string(fima::colors::BOLD) +
                   std::string(fima::colors::UNDERLINE),
                 'r',
                 fs::perms::owner_read);
            show(std::string(fima::colors::YELLOW) + std::string(fima::colors::BOLD) +
                   std::string(fima::colors::UNDERLINE),
                 'w',
                 fs::perms::owner_write);
            show(std::string(fima::colors::GREEN) + std::string(fima::colors::BOLD) +
                   std::string(fima::colors::UNDERLINE),
                 'x',
                 fs::perms::owner_exec);
            show(std::string(fima::colors::RED), 'r', fs::perms::group_read);
            show(std::string(fima::colors::YELLOW), 'w', fs::perms::group_write);
            show(std::string(fima::colors::GREEN), 'x', fs::perms::group_exec);
            show(std::string(fima::colors::RED), 'r', fs::perms::others_read);
            show(std::string(fima::colors::YELLOW), 'w', fs::perms::others_write);
            show(std::string(fima::colors::GREEN), 'x', fs::perms::others_exec);
            std::cout << "  " << entry.string() << (fs::is_directory(st) ? "/" : "") << '\n';
        } catch (const std::exception& ex) {
            fima::helpers::log(
              fima::helpers::logger_type::ERROR, "Failed to get permissions for: ", entry.string());
            fima::helpers::log(fima::helpers::logger_type::ERROR, "", ex.what());
        }
    }
}

} // namespace fima
