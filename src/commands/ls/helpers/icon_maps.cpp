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

#include "helpers/get_data_path.h"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

namespace helpers {

[[nodiscard]] std::string
get_item_icon(const std::filesystem::path& path)
{
    using json = nlohmann::json;
    std::string file_name{ path.filename().string() };

    if (!fs::is_directory(path)) {
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

        fs::path data_path          = fima::helpers::get_application_data_path();
        fs::path fima_data_path     = data_path / "fima";
        fs::path language_file_path = fima_data_path / "map_language_icon.json";

        std::ifstream file_stream(language_file_path);
        file = json::parse(file_stream);

        return file.value(extension, "");
    }

    json file;

    fs::path data_path          = fima::helpers::get_application_data_path();
    fs::path fima_data_path     = data_path / "fima";
    fs::path language_file_path = fima_data_path / "map_directory_icon.json";

    std::ifstream file_stream(language_file_path);
    file = json::parse(file_stream);

    return file.value(file_name, "");
}

} // namespace helpers

} // namespace ls

} // namespace fima
