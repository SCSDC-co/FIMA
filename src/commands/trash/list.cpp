/*
 * src/commands/trash/list.cpp
 * include/commands/trash/list.h
 *
 * The implementation of the `trash list` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/trash/list.h"

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>
#include <toml++/toml.hpp>

#include "fs/operations.h"
#include "fs/trash.h"
#include "theme.h"
#include "utility/get_current_time.h"

namespace fima {

namespace commands {

namespace trash {

void
list()
{

    using namespace ftxui;

    auto to_lower = [=](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    };

    std::vector<std::vector<Element>> table_data{
        { text("ID") | bold | color(fima::theme::theme.primary.get_color_for_tui()),
          text("Size") | bold | color(fima::theme::theme.primary.get_color_for_tui()),
          text("Age") | bold | color(fima::theme::theme.primary.get_color_for_tui()),
          text("Path") | bold | color(fima::theme::theme.primary.get_color_for_tui()) }
    };

    std::vector<std::filesystem::path> paths{};

    for (auto& it : std::filesystem::directory_iterator(
           fima::fs::trash::TRASH_TRASHINFO_PATH,
           std::filesystem::directory_options::skip_permission_denied)) {
        paths.push_back(it.path());
    }

    std::sort(paths.begin(),
              paths.end(),
              [to_lower](const std::filesystem::path& a, const std::filesystem::path& b) {
                  return to_lower(a.filename().string()) < to_lower(b.filename().string());
              });

    for (const auto& path : paths) {
        auto trashinfo{ toml::parse_file(path.string()) };

        auto deleted_at{ *trashinfo["metadata"]["deleted_at"].value<toml::date_time>() };

        auto current_time{ fima::utility::get_current_time() };

        auto deleted_days = std::chrono::sys_days{ std::chrono::year{ deleted_at.date.year } /
                                                   std::chrono::month{ deleted_at.date.month } /
                                                   std::chrono::day{ deleted_at.date.day } };

        auto deleted = std::chrono::system_clock::time_point{ deleted_days } +
                       std::chrono::hours{ deleted_at.time.hour } +
                       std::chrono::minutes{ deleted_at.time.minute } +
                       std::chrono::seconds{ deleted_at.time.second };

        auto age{ std::chrono::duration_cast<std::chrono::seconds>(
          current_time.get_local_time().time_since_epoch() - deleted.time_since_epoch()) };

        std::string id{ fima::fs::trash::get_file_id(path) };
        std::string size{ fima::fs::operations::make_size_readable(
          fima::fs::operations::get_item_size(fima::fs::trash::TRASH_FILES_PATH / path.stem())) };
        std::string age_str{};

        auto seconds{ age.count() };

        if (seconds < 60) {
            age_str = std::format("{} seconds ago", seconds);
        } else if (seconds < 3600) {
            age_str = std::format("{} minutes ago", seconds / 60);
        } else if (seconds < 86400) {
            age_str = std::format("{} hours ago", seconds / 3600);
        } else {
            age_str = std::format("{} days ago", seconds / 86400);
        }

        std::string _path{ *trashinfo["metadata"]["path"].value<std::string>() };

        Color size_color{ fima::theme::theme.info.get_color_for_tui() };
        Decorator size_decorator{ nothing };

        switch (*(size.rbegin() + 1)) {
            case 'K':
                size_decorator = bold;
                break;
            case 'M':
                size_color = fima::theme::theme.warning.get_color_for_tui();
                break;
            case 'G':
                size_color     = fima::theme::theme.warning.get_color_for_tui();
                size_decorator = bold;
                break;
            case 'T':
                size_color = fima::theme::theme.error
                               .get_color_for_tui(); // we use error because i think it's the
                                                     // best one for this kind of info
                break;
            case 'E':
            case 'P':
                size_color     = fima::theme::theme.error.get_color_for_tui();
                size_decorator = bold;
        }

        table_data.push_back(
          { text(id) | color(fima::theme::theme.secondary.get_color_for_tui()),
            text(size) | color(size_color) | size_decorator,
            text(age_str) | color(fima::theme::theme.secondary.get_color_for_tui()),
            text(_path) | color(fima::theme::theme.secondary.get_color_for_tui()) });
    }

    if (table_data.size() == 1) {
        std::cout << fima::theme::theme.primary << "The trash is empty" << fima::theme::Color::reset
                  << '\n';

        return;
    }

    Table table = Table({ table_data });

    table.SelectAll().Border(BorderStyle::ROUNDED);

    TableSelection first_row = table.SelectRow(0);
    first_row.Border(ROUNDED);
    first_row.SeparatorVertical();

    table.SelectColumn(1).DecorateCells(align_right);

    auto table_size{ table_data.size() + 3 };

    Element document = table.Render() | color(fima::theme::theme.border.get_color_for_tui());
    Screen screen = ftxui::Screen::Create(Dimension::Fit(document), Dimension::Fixed(table_size));
    Render(screen, document);
    screen.Print();
    std::cout << '\n';
}

} // namespace trash

} // namespace commands

} // namespace fima
