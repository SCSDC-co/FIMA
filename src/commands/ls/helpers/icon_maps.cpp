/*
 * src/commands/helpers/icon_maps.cpp
 * include/commands/helpers/icon_maps.h
 *
 * An helper to get the correct icon for each file/directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/ls/helpers/icon_maps.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <regex>

#include "program_files.h"

namespace _fs = std::filesystem;

namespace fima {

namespace ls {

namespace helpers {

[[nodiscard]] std::string
get_item_icon(const std::filesystem::path& path)
{
    using json = nlohmann::json;
    std::string file_name{ path.filename().string() };

    if (!_fs::is_directory(path)) {
        if (file_name == ".editorconfig") {
            return "";
        } else if (std::regex_match(file_name, std::regex(R"(^\.git.*)"))) {
            return "";
        } else if (std::regex_match(file_name, std::regex(R"(^\.clang.*)"))) {
            return "󱁻";
        } else if (std::regex_match(file_name, std::regex(R"(license.*)", std::regex::icase))) {
            return "";
        } else if (file_name == "CMakeLists.txt") {
            return "";
        } else if (file_name == "Makefile") {
            return "";
        } else if (std::regex_match(file_name,
                                    std::regex(R"(readme\.(md|markdown))", std::regex::icase))) {
            return "󰂺";
        }

        std::string extension = path.extension();

        json file;

        std::ifstream file_stream(fima::program_files::MAP_LANGUAGES_ICON_PATH);
        file = json::parse(file_stream);

        return file.value(extension, "");
    }

    json file;

    std::ifstream file_stream(fima::program_files::MAP_DIRECTORY_ICON_PATH);
    file = json::parse(file_stream);

    return file.value(file_name, "");
}

} // namespace helpers

} // namespace ls

} // namespace fima
