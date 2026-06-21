/*
 * src/commands/cloc/helpers/print_languages.cpp
 * include/commands/cloc/helpers/print_languages.h
 *
 * This is the logic for printing the languages that cloc supports
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/cloc/helpers/print_languages.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "ftxui/dom/node.hpp"
#include "mappings.h"

namespace fima {

namespace cloc {

namespace helpers {

std::string
join_extensions(const std::vector<std::string>& exts)
{
    std::string out;

    for (size_t i = 0; i < exts.size(); i++) {
        out += exts[i];
        if (i + 1 < exts.size()) {
            out += ", ";
        }
    }

    return out;
}

void
show_languages()
{
    using namespace ftxui;
    using json = nlohmann::json;

    std::map<std::string, std::vector<std::string>> lang_to_exts;

    json file;

    for (auto& [ext, name] : fima::mappings::map_extension_name) {
        lang_to_exts[std::string(name)].push_back(ext);
    }

    for (auto& [lang, exts] : lang_to_exts) {
        std::sort(exts.begin(), exts.end());
    }

    std::vector<std::vector<Element>> table_data;

    table_data.push_back({ text("Language") | bold, text("Extension") | bold });

    for (const auto& [lang, exts] : lang_to_exts) {
        table_data.push_back(
          { text(lang) | color(Color::Green), text(join_extensions(exts)) | color(Color::Blue) });
    }

    table_data.push_back({ text("Total:") | color(Color::Green),
                           text(std::to_string(table_data.size() - 1)) | color(Color::Blue) });

    Table table = Table(table_data);

    table.SelectAll().Border(ROUNDED);

    TableSelection first_row = table.SelectRow(0);
    first_row.Border(ROUNDED);
    first_row.SeparatorVertical();

    table.SelectRows(table_data.size() - 2, -1).SeparatorHorizontal();

    Element document = table.Render() | color(Color::Green);
    Screen screen =
      ftxui::Screen::Create(Dimension::Fit(document), Dimension::Fixed(table_data.size() + 4));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

} // namespace helpers

} // namespace cloc

} // namespace fima
