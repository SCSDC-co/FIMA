/*
 * src/tui/commands/tree/tree_tui.cpp
 * include/tui/commands/tree/tree_tui.h
 *
 * This file defines the TUI for the `tree` subcommand of FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "tui/commands/tree/tree_tui.h"

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

using namespace ftxui;

namespace fima {

namespace tree {
void
tui(const std::filesystem::directory_entry& path,
    const std::vector<ftxui::Element>& tree_vector_tui,
    const size_t& number_of_dirs,
    const size_t& number_of_files)
{
    Element main_box = window(text(" Tree: " + path.path().filename().string() +
                                   (path.path().filename().string().back() == '/' ? " " : "/ ")) |
                                bold,
                              vbox(

                                hbox(text(" "), vbox(tree_vector_tui)),

                                filler(),

                                separator(),

                                vbox(

                                  hbox(text(" Number of directories: ") | color(Color::Green),
                                       text(std::to_string(number_of_dirs)) | color(Color::White)),

                                  hbox(text(" Number of files: ") | color(Color::Green),
                                       text(std::to_string(number_of_files)) | color(Color::White))

                                    ))) |
                       color(Color::Green);

    auto document = main_box;
    auto screen   = Screen::Create(Dimension::Fit(document), Dimension::Full());
    Render(screen, document);
    screen.Print();
    std::cin.get();
}

} // namespace tree

} // namespace fima
