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

#include "commands/info/git.h"

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
#include "mappings.h"
#include "options.h"
#include "theme.h"
#include "utility/join.h"

namespace fima {

namespace info {

void
git(const fima::options::info_options& options, const fima::git::GitRepo& repo)
{
    using namespace ftxui;

    if (!repo.get_is_in_repo()) {
        Element document = text(" NOT IN A REPO ") |
                           color(fima::theme::theme.error.get_color_for_tui()) |
                           borderStyled(ROUNDED, fima::theme::theme.error.get_color_for_tui());

        auto screen = Screen::Create(Dimension::Fit(document));
        Render(screen, document);
        screen.Print();

        std::cout << '\n';

        return;
    }

    auto draw_window_entry = [&](const std::string& title, const Element& value) {
        return hbox(text(title) | bold | color(fima::theme::theme.primary.get_color_for_tui()),
                    value | color(fima::theme::theme.secondary.get_color_for_tui()));
    };

    std::vector<std::string> tag_list;
    std::string tag_list_elements;

    if (options.tags) {
        tag_list = repo.get_tag_list();

        tag_list_elements = fima::utility::join(tag_list, ", ");
    }

    std::vector<fima::git::GitRepo::Remote> remote_list;
    std::vector<std::vector<Element>> remote_table_data;

    if (options.remote) {
        remote_list = repo.get_remote_list();

        std::sort(remote_list.begin(),
                  remote_list.end(),
                  [](const fima::git::GitRepo::Remote& a, const fima::git::GitRepo::Remote& b) {
                      return a.get_name() < b.get_name();
                  });

        remote_list.erase(unique(remote_list.begin(), remote_list.end()), remote_list.end());

        for (int i = 0; i < remote_list.size(); ++i) {
            fima::git::GitRepo::Remote remote{ remote_list[i] };

            remote_table_data.push_back(
              { text(remote.get_name()) | color(fima::theme::theme.secondary.get_color_for_tui()),
                text(" " + remote.get_url()) |
                  color(fima::theme::theme.secondary.get_color_for_tui()) });
        }
    }

    std::vector<Element> document_data = {
        // header
        border(
          hbox(text("GINFO: ") | bold | color(fima::theme::theme.primary.get_color_for_tui()),
               text(repo.get_repo_path()) | color(fima::theme::theme.secondary.get_color_for_tui()),
               text((repo.get_repo_path().string().ends_with("/") ? " " : "/ ")) |
                 color(Color::White) | flex,
               text(std::string(fima::mappings::get_item_icon(repo.get_repo_path())) + " "))) |
          color(fima::theme::theme.border.get_color_for_tui()),

        hbox(window(text(" INFO ") | bold,
                    vbox(draw_window_entry("Branch: ", text(repo.get_repo_branch())),

                         separator(),

                         draw_window_entry("Tag: ", text(repo.get_tag_name())),
                         draw_window_entry("Tag message: ", text(repo.get_tag_message())),
                         draw_window_entry("Tag tagger: ", text(repo.get_tag_tagger())))) |
               color(fima::theme::theme.border.get_color_for_tui()),

             window(text(" COMMITS ") | bold,
                    vbox(draw_window_entry("Commit number: ",
                                           text(std::to_string(repo.get_commit_number()))),

                         separator(),

                         draw_window_entry("Message: ", text(repo.get_commit_message())),
                         draw_window_entry("Commit author: ", text(repo.get_commit_author())),
                         draw_window_entry("Committer: ", text(repo.get_commit_committer())))) |
               color(fima::theme::theme.border.get_color_for_tui()) | flex),

    };

    if (options.tags && !tag_list.empty()) {
        document_data.push_back(window(text(" TAGS ") | bold,
                                       paragraph(tag_list_elements) |
                                         color(fima::theme::theme.secondary.get_color_for_tui())) |
                                color(fima::theme::theme.border.get_color_for_tui()) | flex);
    }

    if (options.remote && !remote_list.empty()) {
        document_data.push_back(
          window(text(" REMOTES ") | bold, Table(remote_table_data).Render()) |
          color(fima::theme::theme.border.get_color_for_tui()) | flex);
    }

    Element document = vbox(document_data);

    Screen screen = Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << '\n';
}

} // namespace info

} // namespace fima
