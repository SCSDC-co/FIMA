/*
 * include/config.h
 * src/config.cpp
 *
 * This file contains the variables that should be available in all the program
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "config.h"

#include <filesystem>
#include <string_view>
#include <toml++/toml.hpp>

#include "fs/get_config_path.h"
#include "mappings.h"
#include "utility/regex.h"

namespace fima {

namespace config {

void
setup_variables()
{
    CONFIG_PATH      = fima::fs::get_application_config_path();
    FIMA_CONFIG_PATH = CONFIG_PATH / "fima";
    CONFIG_FILE_PATH = FIMA_CONFIG_PATH / "config.toml";
    THEME_FILE_PATH  = FIMA_CONFIG_PATH / "theme.toml";
}

void
parse_config_file()
{
    if (!std::filesystem::exists(CONFIG_FILE_PATH)) {
        return;
    }

    auto config{ toml::parse_file(CONFIG_FILE_PATH.string()) };

    depth                  = *config["depth"].value<int>();
    process_directory_size = *config["process_directory_size"].value<bool>();

    if (config["icons"]["files"].is_table()) {
        for (auto& icon : *config["icons"]["files"].as_table()) {
            std::string ext{ std::string(1, '.') + icon.first.data() };
            std::string _icon{ *icon.second.value<std::string>() };

            fima::mappings::map_extension_icon[ext] = _icon;
        }
    }

    if (config["icons"]["dirs"].is_table()) {
        for (auto& icon : *config["icons"]["dirs"].as_table()) {
            std::string ext{ icon.first.data() };
            std::string _icon{ *icon.second.value<std::string>() };

            fima::mappings::map_directory_icon[ext] = _icon;
        }
    }

    if (config["icons"]["name"].is_table()) {
        for (auto& icon : *config["icons"]["name"].as_table()) {
            std::string regex{ fima::utility::regex::glob_to_regex(icon.first.data()) };
            std::string _icon{ *icon.second.value<std::string>() };

            fima::mappings::map_name_icon[regex] = _icon;
        }
    }
}

} // namespace config

} // namespace fima
