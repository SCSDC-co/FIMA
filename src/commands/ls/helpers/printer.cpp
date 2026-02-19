/*
 * src/commands/ls/helpers/printer.cpp
 * include/commands/ls/helpers/printer.h
 *
 * An helper to print the directory entries
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/ls/helpers/printer.h"

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "utility/DirectoryItem.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

namespace helpers {

using namespace ftxui;

void
print_normal(const std::vector<fima::DirectoryItem>& items, const bool& icons)
{
    std::vector<Element> element_vector{};

    for (const fima::DirectoryItem& item : items) {
        std::string final_string;

        final_string += item.get_name(icons);

        if (fs::is_directory(item.get_path())) {
            element_vector.push_back(text(final_string + "  ") | color(Color::Green) | bold);
        } else {
            element_vector.push_back(text(final_string + "  "));
        }
    }

    auto document = hflow({ element_vector });

    auto screen = Screen::Create(Dimension::Fit(document));

    Render(screen, document);
    screen.Print();

    std::cout << '\n';
}

void
print_long(const std::vector<fima::DirectoryItem>& items, const bool& icons, const bool& verbose)
{
    std::vector<std::vector<Element>> table_data{
        { text("Permissions ") | color(Color::Green) | underlined | bold,
          text(" Size ") | color(Color::Yellow) | underlined | bold,
          text(" User ") | color(Color::Red) | underlined | bold,
          text(" Date Modified ") | color(Color::Blue) | underlined | bold,
          text(" Name ") | color(Color::Green) | underlined | bold },
    };

    for (const fima::DirectoryItem& item : items) {
        table_data.push_back(
          { item.get_permissions_tui(),
            text(" " + item.get_size_with_extension() + " ") | color(Color::Yellow),
            text(" " + item.get_user() + " ") | color(Color::Red),
            text(" " + item.get_time() + " ") | color(Color::Blue),
            text(" " + item.get_name(icons) + " ") |
              (fs::is_directory(item.get_path()) ? color(Color::Green) : color(Color::White)) });
    }

    Table table = Table({ table_data });

    int table_size{};

    table_size = table_data.size();

    Element document = table.Render();
    Screen screen = ftxui::Screen::Create(Dimension::Fit(document), Dimension::Fixed(table_size));
    Render(screen, document);
    screen.Print();

    std::cout << '\n';

    if (verbose) {
        int number_of_files       = 0;
        int number_of_directories = 0;

        for (const fima::DirectoryItem& item : items) {
            if (item.is_file()) {
                ++number_of_files;
            } else if (item.is_directory()) {
                ++number_of_directories;
            }
        }

        std::cout << fima::colors::GREEN << "files: " << fima::colors::RESET << number_of_files
                  << fima::colors::GREEN << ", ";
        std::cout << "directories: " << fima::colors::RESET << number_of_directories << '\n';
    }
}

} // namespace helpers

} // namespace ls

} // namespace fima
