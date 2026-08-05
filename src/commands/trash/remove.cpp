/*
 * src/commands/trash/remove.cpp
 * include/commands/trash/remove.h
 *
 * The implementation of the `trash remove` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/trash/remove.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <vector>

#include "fs/trash.h"
#include "theme.h"

namespace fima {

namespace commands {

namespace trash {

void
remove(const std::vector<std::string>& ids)
{
    std::unordered_map<std::string, std::filesystem::path> map_id_path{};

    for (const auto& it :
         std::filesystem::directory_iterator(fima::fs::trash::TRASH_TRASHINFO_PATH)) {
        map_id_path[fima::fs::trash::get_file_id(it.path())] = it.path();
    }

    for (const auto& id : ids) {
        auto it{ map_id_path.find(id) };

        if (it != map_id_path.end()) {
            std::filesystem::path path{ it->second };

            auto parsed_trashinfo{ toml::parse_file(path.string()) };

            std::filesystem::path file_path{ fima::fs::trash::TRASH_FILES_PATH /
                                             path.filename().stem() };
            std::filesystem::path original_path{
                *parsed_trashinfo["metadata"]["path"].value<std::string>()
            };

            std::filesystem::remove(path);
            std::filesystem::remove(file_path);

            std::cout << original_path.string() << fima::theme::theme.primary << " removed."
                      << fima::theme::Color::reset << '\n';
        } else {
            std::cerr << id << fima::theme::theme.error << " isn't a valid ID."
                      << fima::theme::Color::reset << '\n';
        }
    }
}

} // namespace trash

} // namespace commands

} // namespace fima
