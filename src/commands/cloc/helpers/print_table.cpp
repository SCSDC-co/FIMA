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
            const std::string& sorting,
            const bool& quiet)
{
    using namespace ftxui;

    std::vector<std::pair<std::string, fima::cloc::classes::LanguageStats>> sorted;

    for (const auto& it : language_map) {
        sorted.push_back(it);
    }

    if (sorting == "total") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.stats.get_total() > b.second.stats.get_total();
        });
    } else if (sorting == "comments") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.stats.get_comments() > b.second.stats.get_comments();
        });
    } else if (sorting == "blank") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.stats.get_blank_lines() > b.second.stats.get_blank_lines();
        });
    } else if (sorting == "code") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.stats.get_code() > b.second.stats.get_code();
        });
    } else if (sorting == "files") {
        std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
            return a.second.get_files() > b.second.get_files();
        });
    }

    int total_files{ 0 };
    int total_lines{ 0 };
    int total_code{ 0 };
    int total_comment{ 0 };
    int total_blank{ 0 };

    std::vector<std::vector<Element>> table_data;
    std::vector<Element> table_header;

    if (!quiet) {
        table_header = { text("Language") | bold,    text("Files") | bold,
                         text("Total Lines") | bold, text("Code Lines") | bold,
                         text("Comments") | bold,    text("Blank Lines") | bold };
    } else {
        table_header = { text("Language") | bold,
                         text("Total Lines") | bold,
                         text("Code Lines") | bold,
                         text("Comments") | bold,
                         text("Blank Lines") | bold };
    }

    table_data.push_back(table_header);

    for (const auto& [name, language] : sorted) {
        if (!quiet) {
            table_data.push_back(
              { text(name) | color(Color::Green),
                text(std::to_string(language.get_files())) | color(Color::White),
                text(std::to_string(language.stats.get_total())) | color(Color::White),
                text(std::to_string(language.stats.get_code())) | color(Color::White),
                text(std::to_string(language.stats.get_comments())) | color(Color::White),
                text(std::to_string(language.stats.get_blank_lines())) | color(Color::White) });

            total_files += language.get_files();
            total_lines += language.stats.get_total();
            total_code += language.stats.get_code();
            total_comment += language.stats.get_comments();
            total_blank += language.stats.get_blank_lines();
        } else {
            table_data.push_back(
              { text(name) | color(Color::Green),
                text(std::to_string(language.stats.get_total())) | color(Color::White),
                text(std::to_string(language.stats.get_code())) | color(Color::White),
                text(std::to_string(language.stats.get_comments())) | color(Color::White),
                text(std::to_string(language.stats.get_blank_lines())) | color(Color::White) });
        }
    }

    if (!quiet) {
        table_data.push_back({ text("Total") | color(Color::Green),
                               text(std::to_string(total_files)) | color(Color::White),
                               text(std::to_string(total_lines)) | color(Color::White),
                               text(std::to_string(total_code)) | color(Color::White),
                               text(std::to_string(total_comment)) | color(Color::White),
                               text(std::to_string(total_blank)) | color(Color::White) });
    }

    Table table = Table(table_data);

    table.SelectAll().Border(ROUNDED);

    TableSelection first_row = table.SelectRow(0);
    first_row.Border(ROUNDED);
    first_row.SeparatorVertical();

    table.SelectRectangle(1, -1, 0, -1).DecorateCells(align_right);

    if (!quiet) {
        table.SelectRows((table_data.size() - 2), -1).SeparatorHorizontal();
    }

    int table_size;

    if (quiet) {
        table_size = table_data.size() + 3;
    } else {
        table_size = table_data.size() + 4;
    }

    Element document = table.Render() | color(Color::Green);
    Screen screen = ftxui::Screen::Create(Dimension::Fit(document), Dimension::Fixed(table_size));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

} // namespace helpers

} // namespace cloc

} // namespace fima
