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
#include "utility/FileInfo.h"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

namespace helpers {

using namespace ftxui;

void
print_normal(const std::vector<fima::FileInfo>& items, const bool& icons)
{
    std::vector<Element> element_vector{};

    for (const fima::FileInfo& item : items) {
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
print_long(const std::vector<fima::FileInfo>& files, const bool& icons)
{
    std::vector<std::vector<Element>> table_data{
        { text("permissions") | color(Color::Green) | underlined,
          text("  size") | color(Color::Green) | underlined,
          text("  user") | color(Color::Green) | underlined,
          text("  date modified") | color(Color::Green) | underlined,
          text("  name") | color(Color::Green) | underlined },
    };

    for (const fima::FileInfo& file : files) {
        table_data.push_back({ file.get_permissions_tui(),
                               text("  " + file.get_size_with_extension()) | color(Color::Yellow),
                               text("  " + file.get_user()) | color(Color::Red),
                               text("  " + file.get_time()) | color(Color::Blue),
                               text("  " + file.get_name(icons)) |
                                 (fs::is_directory(file.get_path()) ? color(Color::Green) | bold
                                                                    : color(Color::White)) });
    }

    Table table = Table({ table_data });

    int table_size{};

    table_size = table_data.size();

    Element document = table.Render();
    Screen screen = ftxui::Screen::Create(Dimension::Fit(document), Dimension::Fixed(table_size));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

} // namespace helpers

} // namespace ls

} // namespace fima
