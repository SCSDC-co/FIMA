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
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "logger/logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace perms {

std::string
get_perms(const fs::path& item)
{
    std::string perms{};

    try {
        auto st = fs::status(item);

        fs::perms permissions{ st.permissions() };

        auto show = [=](const std::string& color, char op, fs::perms perm) {
            return color + (fs::perms::none == (perm & permissions) ? '-' : op) +
                   fima::colors::RESET;
        };

        auto print_ft = [](const std::string& color, char c) {
            return color + fima::colors::BOLD + c + fima::colors::RESET;
        };

        if (fs::is_regular_file(st)) {
            perms += print_ft("", '-');
        } else if (fs::is_directory(st)) {
            perms += print_ft(fima::colors::GREEN, 'd');
        } else if (fs::is_symlink(st)) {
            perms += print_ft(fima::colors::BLUE, 'l');
        } else if (fs::is_character_file(st)) {
            perms += print_ft("", 'c');
        } else if (fs::is_block_file(st)) {
            perms += print_ft("", 'b');
        } else if (fs::is_fifo(st)) {
            perms += print_ft("", 'p');
        } else if (fs::is_socket(st)) {
            perms += print_ft("", 's');
        } else {
            perms += print_ft("", '?'); // unknown type
        }

        perms += show(fima::colors::RED + fima::colors::BOLD + fima::colors::UNDERLINE,
                      'r',
                      fs::perms::owner_read);
        perms += show(fima::colors::YELLOW + fima::colors::BOLD + fima::colors::UNDERLINE,
                      'w',
                      fs::perms::owner_write);
        perms += show(fima::colors::GREEN + fima::colors::BOLD + fima::colors::UNDERLINE,
                      'x',
                      fs::perms::owner_exec);
        perms += show(fima::colors::RED, 'r', fs::perms::group_read);
        perms += show(fima::colors::YELLOW, 'w', fs::perms::group_write);
        perms += show(fima::colors::GREEN, 'x', fs::perms::group_exec);
        perms += show(fima::colors::RED, 'r', fs::perms::others_read);
        perms += show(fima::colors::YELLOW, 'w', fs::perms::others_write);
        perms += show(fima::colors::GREEN, 'x', fs::perms::others_exec);
    } catch (const std::exception& ex) {
        fima::logger::log(fima::logger::Type::ERROR,
                          true,
                          fima::colors::RED +
                            "Failed to get permissions for: " + fima::colors::RESET + "{}",
                          item.string());
        fima::logger::log(fima::logger::Type::ERROR, true, ex.what());
    }

    return perms;
}

void
print_perms(const std::string& permissions, const fs::path& entry)
{
    std::cout << permissions << "  " << entry.filename().string()
              << (fs::is_directory(entry) ? "/" : "") << '\n';
}

ftxui::Element
get_perms_tui(const std::filesystem::path& item)
{
    using namespace ftxui;

    std::vector<Element> element_vector{};

    try {
        auto st = fs::status(item);

        fs::perms permissions{ st.permissions() };

        auto show = [=](char op, fs::perms perm) {
            return (fs::perms::none == (perm & permissions) ? '-' : op);
        };

        if (fs::is_regular_file(st)) {
            element_vector.push_back(text("-"));
        } else if (fs::is_directory(st)) {
            element_vector.push_back(text("d") | color(Color::Green));
        } else if (fs::is_symlink(st)) {
            element_vector.push_back(text("l") | color(Color::Blue));
        } else if (fs::is_character_file(st)) {
            element_vector.push_back(text("c"));
        } else if (fs::is_block_file(st)) {
            element_vector.push_back(text("b"));
        } else if (fs::is_fifo(st)) {
            element_vector.push_back(text("p"));
        } else if (fs::is_socket(st)) {
            element_vector.push_back(text("s"));
        } else {
            element_vector.push_back(text("?")); // unknown type
        }

        element_vector.push_back(text(std::string(1, show('r', fs::perms::owner_read))) |
                                 color(Color::Red) | bold | underlined);
        element_vector.push_back(text(std::string(1, show('w', fs::perms::owner_write))) |
                                 color(Color::Yellow) | bold | underlined);
        element_vector.push_back(text(std::string(1, show('x', fs::perms::owner_exec))) |
                                 color(Color::Green) | bold | underlined);
        element_vector.push_back(text(std::string(1, show('r', fs::perms::group_read))) |
                                 color(Color::Red));
        element_vector.push_back(text(std::string(1, show('w', fs::perms::group_write))) |
                                 color(Color::Yellow));
        element_vector.push_back(text(std::string(1, show('x', fs::perms::group_exec))) |
                                 color(Color::Green));
        element_vector.push_back(text(std::string(1, show('r', fs::perms::others_read))) |
                                 color(Color::Red));
        element_vector.push_back(text(std::string(1, show('w', fs::perms::others_write))) |
                                 color(Color::Yellow));
        element_vector.push_back(text(std::string(1, show('x', fs::perms::others_exec))) |
                                 color(Color::Green));
    } catch (const std::exception& ex) {
        fima::logger::log(fima::logger::Type::ERROR,
                          true,
                          fima::colors::RED +
                            "Failed to get permissions for: " + fima::colors::RESET + "{}",
                          item.string());
        fima::logger::log(fima::logger::Type::ERROR, true, ex.what());
    }

    Element perms = hbox(element_vector);

    return perms;
}

void
permissions(const std::vector<fs::path>& paths)
{
    for (const fs::path& item : paths) {
        if (!fs::exists(item)) {
            fima::logger::log(fima::logger::Type::ERROR,
                              true,
                              fima::colors::RED +
                                "The path doesn't exists: " + fima::colors::RESET + "{}",
                              item.string());

            continue;
        }

        std::string perms = get_perms(item);

        print_perms(perms, item);
    }
}

} // namespace perms

} // namespace fima
