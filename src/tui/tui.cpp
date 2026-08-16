/*
 * src/tui/tui.cpp
 * include/tui/tui.h
 *
 * This file is the main entry point of the FIMA's TUI
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "tui/tui.h"

#include <algorithm>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

#include "fs/get_directories_entries.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace _fs = std::filesystem;

namespace fima {

namespace tui {

void
start_tui(_fs::path path)
{
    using namespace ftxui;

    std::vector<_fs::directory_entry> list_of_the_directory{
        fima::fs::get_directories_entries_no_git(path, false)
    };

    sort(list_of_the_directory.begin(), list_of_the_directory.end(), [](auto& a, auto& b) {
        if (a.is_directory() && !b.is_directory()) {
            return true;
        }

        if (!a.is_directory() && b.is_directory()) {
            return false;
        }

        return a.path().filename() < b.path().filename();
    });

    std::vector<Element> path_entries;

    for (const _fs::path& entry : list_of_the_directory) {
        auto name = entry.filename().string();

        if (_fs::is_directory(entry)) {
            name += "/";
        }

        path_entries.push_back(
          text(name) | (_fs::is_directory(entry) ? color(Color::Green) : color(Color::White)));
    }

    auto document = vbox({

      hflow({
        text(" PATH ") | border | color(Color::Green),
        text(path.string()) | border | flex | color(Color::Green),
      }),

      border(

        vbox({ vbox(path_entries) })) |
        flex | color(Color::Green),
    });

    auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
    Render(screen, document);
    screen.Print();
}

} // namespace tui

} // namespace fima
