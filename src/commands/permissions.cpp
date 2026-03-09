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
#include <vector>

#include "fs/operations.h"
#include "ftxui/dom/elements.hpp"
#include "logger.h"
#include "utility/colors.h"

namespace fima {

namespace perms {

std::string
get_perms(const std::filesystem::path& item)
{
    std::string perms{};

    try {
        auto st = std::filesystem::status(item);

        std::filesystem::perms permissions{ st.permissions() };

        auto show = [=](const std::string& color, char op, std::filesystem::perms perm) {
            return (std::filesystem::perms::none == (perm & permissions) ? fima::colors::GRAY + "-"
                                                                         : color + op) +
                   fima::colors::RESET;
        };

        auto print_ft = [](const std::string& color, char c) {
            return color + fima::colors::BOLD + c + fima::colors::RESET;
        };

        if (std::filesystem::is_regular_file(st)) {
            perms += print_ft("", '-');
        } else if (std::filesystem::is_directory(st)) {
            perms += print_ft(fima::colors::GREEN, 'd');
        } else if (std::filesystem::is_symlink(st)) {
            perms += print_ft(fima::colors::BLUE, 'l');
        } else if (std::filesystem::is_character_file(st)) {
            perms += print_ft("", 'c');
        } else if (std::filesystem::is_block_file(st)) {
            perms += print_ft("", 'b');
        } else if (std::filesystem::is_fifo(st)) {
            perms += print_ft("", 'p');
        } else if (std::filesystem::is_socket(st)) {
            perms += print_ft("", 's');
        } else {
            perms += print_ft("", '?'); // unknown type
        }

        perms += show(fima::colors::GREEN + fima::colors::BOLD + fima::colors::UNDERLINE,
                      'r',
                      std::filesystem::perms::owner_read);
        perms += show(fima::colors::YELLOW + fima::colors::BOLD + fima::colors::UNDERLINE,
                      'w',
                      std::filesystem::perms::owner_write);
        perms += show(fima::colors::RED + fima::colors::BOLD + fima::colors::UNDERLINE,
                      'x',
                      std::filesystem::perms::owner_exec);
        perms += show(fima::colors::GREEN, 'r', std::filesystem::perms::group_read);
        perms += show(fima::colors::YELLOW, 'w', std::filesystem::perms::group_write);
        perms += show(fima::colors::RED, 'x', std::filesystem::perms::group_exec);
        perms += show(fima::colors::GREEN, 'r', std::filesystem::perms::others_read);
        perms += show(fima::colors::YELLOW, 'w', std::filesystem::perms::others_write);
        perms += show(fima::colors::RED, 'x', std::filesystem::perms::others_exec);
    } catch (const std::exception& ex) {
        fima::logger::error(true,
                            "perms",
                            fima::colors::RED +
                              "Failed to get permissions for: " + fima::colors::RESET + "{}",
                            item.string());
        fima::logger::error(true, "perms", ex.what());
    }

    return perms;
}

ftxui::Element
get_perms_tui(const std::filesystem::path& item)
{
    using namespace ftxui;

    std::vector<Element> element_vector{};

    try {
        auto st = std::filesystem::status(item);

        std::filesystem::perms permissions{ st.permissions() };

        auto show = [=](char op, std::filesystem::perms perm, Color _color) {
            Element element;

            if (std::filesystem::perms::none == (perm & permissions)) {
                element = text("-") | color(Color::GrayDark);
            } else if (perm == std::filesystem::perms::owner_read ||
                       perm == std::filesystem::perms::owner_exec ||
                       perm == std::filesystem::perms::owner_write) {
                element = text(std::string(1, op)) | color(_color) | bold | underlined;
            } else {
                element = text(std::string(1, op)) | color(_color);
            }

            return element;
        };

        if (std::filesystem::is_regular_file(st)) {
            element_vector.push_back(text("-") | bold);
        } else if (std::filesystem::is_directory(st)) {
            element_vector.push_back(text("d") | color(Color::Green));
        } else if (std::filesystem::is_symlink(st)) {
            element_vector.push_back(text("l") | color(Color::Blue));
        } else if (std::filesystem::is_character_file(st)) {
            element_vector.push_back(text("c") | bold);
        } else if (std::filesystem::is_block_file(st)) {
            element_vector.push_back(text("b") | bold);
        } else if (std::filesystem::is_fifo(st)) {
            element_vector.push_back(text("p") | bold);
        } else if (std::filesystem::is_socket(st)) {
            element_vector.push_back(text("s") | bold);
        } else {
            element_vector.push_back(text("?") | bold); // unknown type
        }

        element_vector.push_back(show('r', std::filesystem::perms::owner_read, Color::Green));
        element_vector.push_back(show('w', std::filesystem::perms::owner_write, Color::Yellow));
        element_vector.push_back(show('x', std::filesystem::perms::owner_exec, Color::Red));
        element_vector.push_back(show('r', std::filesystem::perms::group_read, Color::Green));
        element_vector.push_back(show('w', std::filesystem::perms::group_write, Color::Yellow));
        element_vector.push_back(show('x', std::filesystem::perms::group_exec, Color::Red));
        element_vector.push_back(show('r', std::filesystem::perms::others_read, Color::Green));
        element_vector.push_back(show('w', std::filesystem::perms::others_write, Color::Yellow));
        element_vector.push_back(show('x', std::filesystem::perms::others_exec, Color::Red));
    } catch (const std::exception& ex) {
        fima::logger::error(true,
                            "perms",
                            fima::colors::RED +
                              "Failed to get permissions for: " + fima::colors::RESET + "{}",
                            item.string());
        fima::logger::error(true, "perms", ex.what());
    }

    Element perms = hbox(element_vector);

    return perms;
}

void
print_perms(const std::string& permissions, const std::filesystem::path& entry)
{
    std::cout << permissions << "  "
              << (std::filesystem::is_directory(entry)              ? fima::colors::GREEN
                  : fima::fs::operations::is_file_executable(entry) ? fima::colors::RED
                                                                    : "")
              << entry.filename().string() << (std::filesystem::is_directory(entry) ? "/" : "")
              << fima::colors::RESET << '\n';
}

} // namespace perms

namespace commands {

void
permissions(const std::vector<std::filesystem::path>& paths)
{
    for (const std::filesystem::path& item : paths) {
        if (!std::filesystem::exists(item)) {
            fima::logger::error(true,
                                "perms",
                                fima::colors::RED +
                                  "The path doesn't exists: " + fima::colors::RESET + "{}",
                                item.string());

            continue;
        }

        std::string perms = fima::perms::get_perms(item);

        fima::perms::print_perms(perms, item);

        fima::logger::info(false, "perms", "Got perms for: {}", item.string());
    }
}

} // namespace commands

} // namespace fima
