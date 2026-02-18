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
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "commands/ls/helpers/icon_maps.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

namespace helpers {

void
print(const std::vector<fs::path>& items, const bool& icons)
{
    using namespace ftxui;

    std::vector<Element> element_vector{};

    for (const fs::path& item : items) {
        std::string final_string;

        if (icons) {
            final_string += get_item_icon(item) + " ";
        }

        final_string += item.string();

        if (fs::is_directory(item)) {
            final_string += "/";

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

} // namespace helpers

} // namespace ls

} // namespace fima
