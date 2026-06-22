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

#include <algorithm>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>
#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>
#include <vector>

#include "fs/DirectoryItem.h"
#include "fs/operations.h"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fima {

namespace ls {

namespace helpers {

using namespace ftxui;

// what the actual fuck is this function, I don't understand nothing
void
print_normal(const std::vector<fima::fs::DirectoryItem>& items, const bool& icons)
{
    int max_width = 0;

    for (const auto& item : items) {
        max_width = std::max(max_width, static_cast<int>(item.get_name(icons).size()));
    }

    int term_width = Terminal::Size().dimx;
    int cols       = std::max(2, term_width / max_width);
    int rows       = (items.size() + cols - 1) / cols;

    std::vector<std::vector<std::string>> grid_string(rows);
    std::vector<std::vector<Element>> grid(rows);

    for (size_t i = 0; i < items.size(); ++i) {
        size_t row = i % rows;

        std::string element_string = items[i].get_name(icons);

        grid_string[row].push_back(element_string);
    }

    auto get_max_of_column = [&](int column) {
        int max_length = 0;

        for (size_t row = 0; row < grid_string.size(); ++row) {
            if (column < grid_string[row].size()) {
                max_length =
                  std::max(max_length, static_cast<int>(grid_string[row][column].size()));
            }
        }

        return max_length;
    };

    for (size_t i = 0; i < items.size(); ++i) {
        size_t row = i % rows;
        size_t col = i / rows;

        std::string element_string = items[i].get_name(icons);

        element_string +=
          std::string(std::max(static_cast<int>(element_string.size()), get_max_of_column(col)) -
                        element_string.size() + 2,
                      ' ');

        Element element = text(element_string) | color(items[i].get_color_tui());

        if (items[i].is_directory() ||
            fima::fs::operations::is_file_executable(items[i].get_path())) {
            element = element | bold;
        }

        grid[row].push_back(element);
    }

    auto document = gridbox(grid);
    auto screen   = Screen::Create(Dimension::Full(), Dimension::Fixed(rows));
    Render(screen, document);
    screen.Print();

    std::cout << '\n';
}

void
print_long(std::vector<fima::fs::DirectoryItem>& items,
           const bool& icons,
           const bool& verbose,
           const bool& headers)
{
    std::vector<std::vector<Element>> table_data{};

    if (headers) {
        table_data.push_back({ text("Permissions ") | color(Color::Yellow) | bold,
                               text("Size ") | color(Color::Green) | bold | align_right,
                               text("User ") | color(Color::Red) | bold,
                               text("Date Modified ") | color(Color::Blue) | bold,
                               text("Name ") | color(Color::Green) | bold });
    }

    for (fima::fs::DirectoryItem& item : items) {
        Color item_color{ item.get_color_tui() };
        Color size_color{ Color::Green };
        Decorator size_decorator{ nothing };

        item.set_size();

        std::string size{ item.get_size_with_extension() };

        switch (*(size.rbegin() + 1)) {
            case 'K':
                size_decorator = bold;
                break;
            case 'M':
                size_color = Color::Yellow;
                break;
            case 'G':
                size_color     = Color::Yellow;
                size_decorator = bold;
                break;
            case 'T':
                size_color = Color::Red;
                break;
            case 'E':
            case 'P':
                size_color     = Color::Red;
                size_decorator = bold;
        }

        table_data.push_back(
          { hbox(item.get_permissions_tui(), text(" ")),
            text(size + " ") | color(size_color) | size_decorator | align_right,
            text(item.get_owner() + " ") | color(Color::Red),
            text(item.get_last_modification_date() + " ") | color(Color::Blue),
            hbox(text(item.get_name(icons)) |
                   (fima::fs::operations::is_file_executable(item.get_path()) ||
                        std::filesystem::is_directory(item.get_path())
                      ? color(item_color) | bold
                      : color(item_color)),
                 text((item.is_symlink() ? " -> " + std::string(item.get_symlink_target()) : "")) |
                   color(item_color)) });
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

        for (const fima::fs::DirectoryItem& item : items) {
            if (item.is_directory()) {
                ++number_of_directories;
            } else if (item.is_file()) {
                ++number_of_files;
            }
        }

        std::cout << termcolor::green << "files: " << termcolor::reset << number_of_files
                  << termcolor::green << ", ";
        std::cout << "directories: " << termcolor::reset << number_of_directories << '\n';
    }
}

} // namespace helpers

} // namespace ls

} // namespace fima
