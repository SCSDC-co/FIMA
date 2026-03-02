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

#include <algorithm>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "git/GitRepo.h"
#include "program_files.h"
#include "utility/join.h"

namespace fima {

namespace info {

namespace git {

void
info(const std::filesystem::directory_entry& path, const fima::git::GitRepo& repo)
{
    using namespace ftxui;

    if (!repo.get_is_in_repo()) {
        Element document =
          text(" NOT IN A REPO ") | color(Color::Red) | borderStyled(ROUNDED, Color::Red);

        auto screen = Screen::Create(Dimension::Fit(document));
        Render(screen, document);
        screen.Print();

        std::cout << '\n';

        return;
    }

    auto draw_window_entry = [&](const std::string& title, const Element& value) {
        return hbox(text(title) | bold | color(Color::Green), value | color(Color::White));
    };

    std::vector<std::string> tag_list = repo.get_tag_list();

    std::string tag_list_elements{ fima::utility::join(tag_list, ", ") };

    std::vector<fima::git::GitRepo::Remote> remote_list{ repo.get_remote_list() };

    std::vector<std::vector<Element>> remote_table_data;

    std::sort(remote_list.begin(),
              remote_list.end(),
              [](const fima::git::GitRepo::Remote& a, const fima::git::GitRepo::Remote& b) {
                  return a.get_name() < b.get_name();
              });

    for (int i = 0; i < remote_list.size(); ++i) {
        fima::git::GitRepo::Remote remote{ remote_list[i] };

        std::vector<std::string> remote_fetch_refspec{ remote.get_fetch_refspec() };
        std::vector<std::string> remote_push_refspec{ remote.get_push_refspec() };

        remote_table_data.push_back(
          { text(remote.get_name()) | color(Color::White),
            text(" " + remote.get_url()) | color(Color::White),
            text((!remote_fetch_refspec.empty() ? " (fetch)" : "")) | color(Color::White),
            text((!remote_push_refspec.empty() ? " (push)" : "")) | color(Color::White) });
    }

    Element document = vbox(
      { // header
        border(hbox(text("GINFO: ") | bold | color(Color::Green),
                    text(repo.get_repo_path()) | color(Color::White),
                    text((repo.get_repo_path().string().ends_with("/") ? " " : "/ ")) |
                      color(Color::White) | flex,
                    text(fima::program_files::get_item_icon(repo.get_repo_path()) + " "))) |
          color(Color::Green),

        hbox(window(text(" INFO ") | bold,
                    vbox(draw_window_entry("Branch: ", text(repo.get_repo_branch())),

                         separator(),

                         draw_window_entry("Tag: ", text(repo.get_tag_name())),
                         draw_window_entry("Tag message: ", text(repo.get_tag_message())),
                         draw_window_entry("Tag tagger: ", text(repo.get_tag_tagger())))) |
               color(Color::Green),

             window(text(" COMMITS ") | bold,
                    vbox(draw_window_entry("Commit number: ",
                                           text(std::to_string(repo.get_commit_number()))),

                         separator(),

                         draw_window_entry("Message: ", text(repo.get_commit_message())),
                         draw_window_entry("Commit author: ", text(repo.get_commit_author())),
                         draw_window_entry("Committer: ", text(repo.get_commit_committer())))) |
               color(Color::Green) | flex),

        window(text(" TAGS ") | bold,
               (!tag_list.empty() ? paragraph(tag_list_elements) | color(Color::White)
                                  : text("This repo doesn't have tags") | color(Color::White))) |
          color(Color::Green) | flex,

        window(text(" REMOTES ") | bold, Table(remote_table_data).Render()) | color(Color::Green) |
          flex });

    Screen screen = Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << '\n';
}

} // namespace git

} // namespace info

} // namespace fima
