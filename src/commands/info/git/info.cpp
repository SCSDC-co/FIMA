/*
 * src/commands/info/git/info.cpp
 * include/commands/info/git/info.h
 *
 * The logic for getting and printing info about a git repo
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

#include "commands/ls/helpers/icon_maps.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "git/GitRepo.h"

namespace fima {

namespace info {

namespace git {

void
info(const std::filesystem::directory_entry& path, const fima::git::GitRepo& repo)
{
    using namespace ftxui;

    auto draw_window_entry = [&](const std::string& title, const Element& value) {
        return hbox(text(title) | bold | color(Color::Green), value | color(Color::White));
    };

    Element document = vbox({
      // header
      border(
        hbox(text("GINFO: ") | bold | color(Color::Green),
             text(repo.get_repo_path()) | color(Color::White),
             text((repo.get_repo_path().ends_with("/") ? " " : "/ ")) | color(Color::White) | flex,
             text(fima::ls::helpers::get_item_icon(repo.get_repo_path()) + " "))) |
        color(Color::Green),

      window(
        text(" INFO ") | bold,
        vbox(draw_window_entry("Branch: ", text(repo.get_repo_branch())),
             draw_window_entry("Commit number: ", text(std::to_string(repo.get_commit_number()))),
             draw_window_entry("Message: ", text(repo.get_commit_message())),
             draw_window_entry("Commit author: ", text(repo.get_commit_author())),
             draw_window_entry("Committer: ", text(repo.get_commit_committer())))) |
        color(Color::Green) | flex,
    });

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();

    std::cout << '\n';
}

} // namespace git

} // namespace info

} // namespace fima
