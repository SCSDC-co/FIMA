/*
 * src/commands/info/file.cpp
 * include/commands/info/file.h
 *
 * The logic for getting and printing info about a file
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/info/file.h"

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>

#include "fs/File.h"
#include "ftxui/dom/node.hpp"
#include "theme.h"

namespace fima {

namespace info {

void
file(const std::filesystem::directory_entry& path)
{
    using namespace ftxui;

    fima::fs::File file(path);

    file.metadata.set_size();
    file.set_stats();

    auto draw_window_entry = [&](const std::string& title, const Element& value) {
        return hbox(text(title) | bold | color(fima::theme::theme.primary.get_color_for_tui()),
                    value | color(fima::theme::theme.secondary.get_color_for_tui()));
    };

    Element document = vbox(
      { // header
        border(hbox(
          text("FINFO: ") | bold | color(fima::theme::theme.primary.get_color_for_tui()),
          text(file.metadata.get_path()) | color(fima::theme::theme.secondary.get_color_for_tui()),
          text((file.metadata.is_symlink() ? " -> " : "")),
          text((file.metadata.is_symlink() ? file.metadata.get_symlink_target().string() : "")) |
            color(fima::theme::theme.symlink.get_color_for_tui()),
          text((file.metadata.get_is_hidden() ? " (hidden)" : "")) | flex,
          text("  " + file.metadata.get_icon() + " "))) |
          color(fima::theme::theme.border.get_color_for_tui()),

        hbox(
          window(text(" INFO ") | bold,
                 vbox(draw_window_entry("Size: ", text(file.metadata.get_size_with_extension())),
                      draw_window_entry("Permissions: ", file.metadata.get_permissions_tui()),
                      draw_window_entry("File type: ", text(file.get_file_type())),
                      draw_window_entry("Last modification date: ",
                                        text(file.metadata.get_last_modification_date())))) |
            color(fima::theme::theme.border.get_color_for_tui()) | flex,

          window(
            text(" LOC ") | bold,
            vbox(draw_window_entry("Code: ", text(std::to_string(file.stats.get_code()))),
                 draw_window_entry("Comments: ", text(std::to_string(file.stats.get_comments()))),
                 draw_window_entry("Blank lines: ",
                                   text(std::to_string(file.stats.get_blank_lines()))),
                 draw_window_entry("Total: ", text(std::to_string(file.stats.get_total()))))) |
            color(fima::theme::theme.border.get_color_for_tui())) });

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();

    std::cout << '\n';
}

} // namespace info

} // namespace fima
