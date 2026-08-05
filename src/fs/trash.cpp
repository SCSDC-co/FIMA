/*
 * src/fs/trash.cpp
 * include/fs/trash.h
 *
 * Trash system for FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/trash.h"

#include <cstddef>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

#include "fs/get_application_directories.h"
#include "theme.h"
#include "utility/get_current_time.h"

namespace fima {

namespace fs {

namespace trash {

void
setup_variables()
{
    TRASH_PATH           = fima::fs::get_application_data_path() / "fima" / "trash";
    TRASH_FILES_PATH     = TRASH_PATH / "files";
    TRASH_TRASHINFO_PATH = TRASH_PATH / "trashinfo";

    std::filesystem::create_directories(TRASH_FILES_PATH);
    std::filesystem::create_directories(TRASH_TRASHINFO_PATH);
}

std::string
get_file_id(const std::filesystem::path& path)
{
    std::string id{};

    std::string file_name{ path.filename().string() };

    std::size_t underscores = 0;

    for (std::size_t i = 0; i < file_name.size(); ++i) {
        if (file_name[i] == '_') {
            ++underscores;

            if (underscores == 3) {
                id = file_name.substr(0, i);
            }
        }
    }

    return id;
}

void
add_file_to_trash(const std::filesystem::path& path)
{
    auto to_lower = [=](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    };

    auto current_time{ fima::utility::get_current_time() };

    auto local_time = current_time.get_local_time();

    auto days = std::chrono::floor<std::chrono::days>(local_time);
    std::chrono::year_month_day ymd{ days };
    std::chrono::hh_mm_ss hms{ local_time - days };

    toml::date_time deleted_at{ toml::date{ static_cast<int>(ymd.year()),
                                            static_cast<unsigned>(ymd.month()),
                                            static_cast<unsigned>(ymd.day()) },
                                toml::time{ static_cast<int>(hms.hours().count()),
                                            static_cast<int>(hms.minutes().count()),
                                            static_cast<int>(hms.seconds().count()) } };

    std::string current_time_formatted{ std::format("{:%Y%m%d_%H%M%S}", current_time) };

    std::string file_name{ path.filename().string() };

    std::vector<std::filesystem::path> paths{};

    for (auto& it : std::filesystem::directory_iterator(
           fima::fs::trash::TRASH_FILES_PATH,
           std::filesystem::directory_options::skip_permission_denied)) {
        paths.push_back(it.path());
    }

    std::sort(paths.begin(),
              paths.end(),
              [to_lower](const std::filesystem::path& a, const std::filesystem::path& b) {
                  return to_lower(a.filename().string()) < to_lower(b.filename().string());
              });

    std::size_t i{ 0 };

    for (const auto& path : paths) {
        std::string full_id{ get_file_id(path) };

        int counter{ std::stoi(full_id.substr(16)) };

        if (full_id.substr(0, 15) == current_time_formatted) {
            if (i == counter) {
                ++i;
            }
        }
    }

    std::filesystem::path trash_path =
      TRASH_FILES_PATH / (current_time_formatted + '_' + std::to_string(i) + '_' + file_name);

    std::filesystem::path trashinfo_path{ TRASH_TRASHINFO_PATH /
                                          (trash_path.filename().string() + ".trashinfo") };

    try {
        std::filesystem::rename(path, trash_path);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << fima::theme::theme.error << "An error occured when moving the file "
                  << fima::theme::Color::reset << path.string() << fima::theme::theme.error << ": "
                  << fima::theme::Color::reset << '\n';

        std::cerr << e.what();

        return;
    }

    auto tbl{ toml::table{
      { "metadata",
        toml::table{
          { "path", std::filesystem::absolute(std::filesystem::weakly_canonical(path)).string() },
          { "deleted_at", deleted_at } } } } };

    std::ofstream trashinfo_file{ trashinfo_path };

    if (!trashinfo_file.is_open()) {
        std::filesystem::rename(trash_path, path);

        std::cerr << fima::theme::theme.error
                  << "An error occoured when creating trashinfo file: " << fima::theme::Color::reset
                  << trashinfo_path.string() << fima::theme::theme.error << '.'
                  << fima::theme::Color::reset << '\n';

        return;
    }

    trashinfo_file << tbl;

    trashinfo_file.close();
}

} // namespace trash

} // namespace fs

} // namespace fima
