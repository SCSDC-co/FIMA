/*
 * src/commands/cloc/helpers/print_table.cpp
 * include/commands/cloc/helpers/print_table.h
 *
 * This is the logic for printing the cloc table
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/cloc/helpers/print_table.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "commands/cloc/helpers/LanguageStats.h"
#include "ftxui/dom/node.hpp"

namespace fima {

namespace cloc {

namespace helpers {

void
print_table(const std::unordered_map<std::string, fima::cloc::classes::LanguageStats>& language_map,
            const std::string& sorting)
{
    using namespace ftxui;

    std::vector<std::pair<std::string, fima::cloc::classes::LanguageStats>> sorted;

    for (const auto& it : language_map) {
        sorted.push_back(it);
    }

    if (sorting == "total") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.get_total() > b.second.get_total();
        });
    } else if (sorting == "comments") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.get_comment() > b.second.get_comment();
        });
    } else if (sorting == "blank") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.get_blank() > b.second.get_blank();
        });
    } else if (sorting == "code") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.get_code() > b.second.get_code();
        });
    } else if (sorting == "files") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.get_files() > b.second.get_files();
        });
    }

    std::vector<std::vector<Element>> table_data;

    std::vector<Element> table_header = { text("Language") | bold,    text("Files") | bold,
                                          text("Total Lines") | bold, text("Code Lines") | bold,
                                          text("Comments") | bold,    text("Blank Lines") | bold };

    table_data.push_back(table_header);

    for (const auto& [name, stats] : sorted) {
        table_data.push_back({ text(name) | color(Color::Green),
                               text(std::to_string(stats.get_files())) | color(Color::White),
                               text(std::to_string(stats.get_total())) | color(Color::White),
                               text(std::to_string(stats.get_code())) | color(Color::White),
                               text(std::to_string(stats.get_comment())) | color(Color::White),
                               text(std::to_string(stats.get_blank())) | color(Color::White) });
    }

    Table table = Table(table_data);

    table.SelectAll().Border(ROUNDED);

    TableSelection first_row = table.SelectRow(0);
    first_row.Border(ROUNDED);
    first_row.SeparatorVertical();

    table.SelectRectangle(1, -1, 0, -1).DecorateCells(align_right);

    Element document = table.Render() | color(Color::Green);
    Screen screen =
      ftxui::Screen::Create(Dimension::Fit(document), Dimension::Fixed(table_data.size() + 3));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

}

}

}
