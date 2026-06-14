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

#include "commands/info/directory.h"

#include <cmath>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "config.h"
#include "fs/Directory.h"
#include "fs/operations.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "git/GitRepo.h"
#include "utility/join.h"
#include "utility/most_common.h"
#include "utility/regex.h"

namespace fima {

namespace info {

void
dir(const std::filesystem::directory_entry& path,
    const bool& verbose,
    const fima::git::GitRepo& repo)
{
    using namespace ftxui;

    fima::fs::Directory dir(path);

    dir.metadata.set_size();
    dir.set_stats(repo);
    dir.set_number_of_files(repo);

    auto draw_window_entry = [&](const std::string& title, const Element& value) {
        return hbox(text(title) | bold | color(Color::Green), value | color(Color::White));
    };

    Element document{};

    if (verbose) {
        auto it{ std::filesystem::recursive_directory_iterator(
          path, std::filesystem::directory_options::skip_permission_denied) };

        std::unordered_set<std::string> extensions_set{};
        std::vector<std::string> extensions_vec{};

        std::map<size_t, std::filesystem::path> size_path_map{};

        for (auto& item : it) {
            if (item.is_directory()) {
                continue;
            }

            if (repo.is_file_ignored(item) &&
                fima::utility::regex::matches_any_regex(item.path().filename().string(),
                                                        fima::config::DEFAULT_DIRS_TO_IGNORE)) {
                it.disable_recursion_pending();
                continue;
            }

            std::filesystem::path path = item.path();
            std::string extension      = path.extension().string();

            size_t size = fima::fs::operations::get_item_size(path);

            size_path_map[size] = path;

            if (extension != "") {
                extensions_set.insert(extension);
                extensions_vec.push_back(extension);
            }
        }

        std::string extensions{ fima::utility::join(extensions_set, ", ") };

        std::string most_common_extension{ fima::utility::most_common(extensions_vec.begin(),
                                                                      extensions_vec.end()) };

        auto biggest_file_size_tmp{ std::prev(size_path_map.end())->first };

        std::string biggest_file_path{ std::filesystem::relative(
          std::prev(size_path_map.end())->second) };
        std::string biggest_file_size{};

        if (biggest_file_size_tmp == 0) {
            biggest_file_size = "-";
        }

        double dimension = static_cast<double>(biggest_file_size_tmp);
        std::string ext;

        if (dimension >= 1024 * 1024 * 1024) {
            ext = "GB";
            dimension /= 1024 * 1024 * 1024;
        } else if (dimension >= 1024 * 1024) {
            ext = "MB";
            dimension /= 1024 * 1024;
        } else if (dimension >= 1024) {
            ext = "KB";
            dimension /= 1024;
        } else {
            ext = "B";
        }

        // if the number is already rounded there's no need to display the decimal digits
        if (std::round(dimension) == static_cast<int>(dimension)) {
            biggest_file_size = std::format("{:.0f}{}", dimension, ext);
        } else {
            biggest_file_size = std::format("{:.2f}{}", dimension, ext);
        }

        document = vbox(
          // header
          border(hbox(text("DINFO: ") | bold | color(Color::Green),
                      text(dir.metadata.get_path()) | color(Color::White),
                      text((dir.metadata.get_path().string().ends_with("/") ? "" : "/")) |
                        color(Color::White),
                      text((dir.metadata.get_is_hidden() ? " (hidden) " : " ")) | flex,
                      text(dir.metadata.get_icon()))) |
            color(Color::Green),

          hbox(
            window(text(" INFO ") | bold,
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
              color(Color::Green)),
          window(text(" VERBOSE ") | bold | color(Color::Green),
                 vbox(draw_window_entry("Extensions: ", text(extensions)),
                      draw_window_entry("Most common extension: ", text(most_common_extension)),
                      draw_window_entry(
                        "Biggest file: ",
                        hbox(text(biggest_file_path), text(" (" + biggest_file_size + ")"))))) |
            color(Color::Green));
    } else {
        document = vbox(
          // header
          border(hbox(text("DINFO: ") | bold | color(Color::Green),
                      text(dir.metadata.get_path()) | color(Color::White),
                      text((dir.metadata.get_path().string().ends_with("/") ? "" : "/")) |
                        color(Color::White),
                      text((dir.metadata.get_is_hidden() ? " (hidden) " : " ")) | flex,
                      text(dir.metadata.get_icon()))) |
            color(Color::Green),

          hbox(
            window(text(" INFO ") | bold,
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
              color(Color::Green)));
    }

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();

    std::cout << '\n';
}

} // namespace info

} // namespace fima
