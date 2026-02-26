/*
 * src/commands/info/directory.cpp
 * include/commands/info/directory.h
 *
 * The logic for getting and printing info about a directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>

#include "fs/Directory.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fima {

namespace info {

namespace dir {

void
info(const std::filesystem::directory_entry& path)
{
    using namespace ftxui;

    fima::fs::Directory dir(path);

    dir.metadata.set_size();
    dir.set_stats();
    dir.set_number_of_files();

    auto draw_window_entry = [&](const std::string& title, const Element& value) {
        return hbox(text(title) | bold | color(Color::Green), value | color(Color::White));
    };

    Element document = vbox(
      { // header
        border(hbox(text("DINFO: ") | bold | color(Color::Green),
                    text(dir.metadata.get_path()) | color(Color::White),
                    text((dir.metadata.get_path().string().ends_with("/") ? "" : "/")) |
                      color(Color::White),
                    text((dir.metadata.get_is_hidden() ? " (hidden) " : " ")) | flex,
                    text(dir.metadata.get_icon()))) |
          color(Color::Green),

        hbox(window(text(" INFO ") | bold,
                    vbox(draw_window_entry("Size: ", text(dir.metadata.get_size_with_extension())),
                         draw_window_entry("Permissions: ", dir.metadata.get_permissions_tui()),
                         draw_window_entry("Number of files: ",
                                           text(std::to_string(dir.get_number_of_files()))),
                         draw_window_entry("Last modification date: ",
                                           text(dir.metadata.get_last_modification_date())))) |
               color(Color::Green) | flex,

             window(
               text(" LOC ") | bold,
               vbox(draw_window_entry("Code: ", text(std::to_string(dir.stats.get_code()))),
                    draw_window_entry("Comments: ", text(std::to_string(dir.stats.get_comments()))),
                    draw_window_entry("Blank lines: ",
                                      text(std::to_string(dir.stats.get_blank_lines()))),
                    draw_window_entry("Total: ", text(std::to_string(dir.stats.get_total()))))) |
               color(Color::Green)) });

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();

    std::cout << '\n';
}

} // namespace dir

} // namespace info

} // namespace fima
