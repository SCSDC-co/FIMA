/*
 * src/program_files.cpp
 * include/program_files.h
 *
 * A utility to setup the program files (like the config file, and the JSON files)
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "program_files.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <unordered_set>

#include "fs/get_config_path.h"
#include "fs/operations.h"
#include "termcolor/termcolor.hpp"

namespace fima {

namespace program_files {

[[nodiscard]] nlohmann::json
parse_file(const std::filesystem::path& path)
{
    std::ifstream file_stream(path);

    return nlohmann::json::parse(file_stream);
}

[[nodiscard]] std::string
get_language_name(const std::filesystem::path& path)
{
    std::string filename = path.filename().string();

    static const std::unordered_set<std::string> special_shell = {
        ".gitignore",     ".clangd",    ".clang-format", ".editorconfig", ".git-blame-ignore-revs",
        ".gitattributes", ".clang-tidy"
    };

    if (filename == "CMakeLists.txt") {
        return "CMake";
    } else if (filename == "LICENSE" || filename == "license") {
        return "LICENSE";
    } else if (filename == "Makefile" || filename == "BSDmakefile") {
        return "Make";
    } else if (special_shell.contains(filename)) {
        return "YAML";
    }

    std::string extension = path.extension();

    return map_language_name_json.value(extension, "Text");
}

[[nodiscard]] std::string
get_language_family(const std::filesystem::path& path)
{
    std::string filename = path.filename().string();

    static const std::unordered_set<std::string> special_shell_type = {
        "CMakeLists.txt", ".gitignore",  ".clangd", ".clang-format", ".editorconfig",
        "Makefile",       "BSDmakefile", ".bashrc", ".zshrc",        ".styluaignore"
    };

    static const std::unordered_set<std::string> special_text_type = { "LICENSE",
                                                                       "license",
                                                                       ".mailmap" };

    static const std::unordered_set<std::string> special_lua_type = {
        ".luacov",
        ".luacheckrc",
    };

    if (special_text_type.contains(filename)) {
        return "text";
    }

    if (special_shell_type.contains(filename)) {
        return "shell_like";
    }

    if (special_lua_type.contains(filename)) {
        return "lua";
    }

    std::string extension = path.extension();

    return map_languages_family_json.value(extension, "text");
}

[[nodiscard]] std::string
get_item_icon(const std::filesystem::path& path)
{
    if (std::filesystem::is_symlink(path)) {
        return " ";
    }

    std::string file_name{ path.filename().string() };

    if (!std::filesystem::is_directory(path)) {
        if (file_name == ".editorconfig") {
            return "";
        } else if (std::regex_match(file_name, std::regex(R"(^\.git.*)"))) {
            return "";
        } else if (std::regex_match(file_name, std::regex(R"(^\.clang.*)"))) {
            return "󱁻";
        } else if (std::regex_match(file_name, std::regex(R"(license.*)", std::regex::icase))) {
            return "";
        } else if (std::regex_match(file_name,
                                    std::regex(R"(readme\.(md|markdown))", std::regex::icase))) {
            return "󰂺";
        } else if (std::regex_match(file_name,
                                    std::regex(R"((\.)?(bash|zsh)rc)", std::regex::icase))) {
            return "󱆃";
        } else if (file_name == "CMakeLists.txt") {
            return "";
        } else if (file_name == "Makefile") {
            return "";
        }

        std::string extension = path.extension();

        return map_language_icon_json.value(extension, "");
    }

    if (std::filesystem::is_empty(path)) {
        return "";
    }

    return map_directory_icon_json.value(file_name, "");
}

void
create_config_dirs()
{
    if (!std::filesystem::exists(FIMA_CONFIG_PATH)) {
        std::filesystem::create_directory(FIMA_CONFIG_PATH);
    }

    if (!std::filesystem::exists(MAPPINGS_PATH)) {
        std::filesystem::create_directory(MAPPINGS_PATH);
    }
}

void
create_config_files()
{
    create_config_dirs();

    if (!std::filesystem::exists(CONFIG_FILE_PATH)) {
        fima::fs::operations::create(CONFIG_FILE_PATH, "");
    }

    if (!std::filesystem::exists(FAMILY_SPEC_PATH)) {
        fima::fs::operations::create(FAMILY_SPEC_PATH, family_spec_file);
    }

    if (!std::filesystem::exists(MAP_LANGUAGES_ICON_PATH)) {
        fima::fs::operations::create(MAP_LANGUAGES_ICON_PATH, map_file_icon);
    }

    if (!std::filesystem::exists(MAP_DIRECTORY_ICON_PATH)) {
        fima::fs::operations::create(MAP_DIRECTORY_ICON_PATH, map_directory_icon);
    }

    if (!std::filesystem::exists(MAP_LANGUAGES_FAMILY_PATH)) {
        fima::fs::operations::create(MAP_LANGUAGES_FAMILY_PATH, map_languages_family);
    }

    if (!std::filesystem::exists(MAP_LANGUAGES_NAME_PATH)) {
        fima::fs::operations::create(MAP_LANGUAGES_NAME_PATH, map_languages_name);
    }
}

void
reset_config_files(const bool& preserve_config_file)
{
    create_config_dirs();

    if (!preserve_config_file) {
        fima::fs::operations::overwrite(CONFIG_FILE_PATH, "");
    }

    fima::fs::operations::overwrite(FAMILY_SPEC_PATH, family_spec_file);
    fima::fs::operations::overwrite(MAP_LANGUAGES_ICON_PATH, map_file_icon);
    fima::fs::operations::overwrite(MAP_DIRECTORY_ICON_PATH, map_directory_icon);
    fima::fs::operations::overwrite(MAP_LANGUAGES_FAMILY_PATH, map_languages_family);
    fima::fs::operations::overwrite(MAP_LANGUAGES_NAME_PATH, map_languages_name);

    std::cout << termcolor::green
              << "Successfully reset all the config file, config file preserved: "
              << termcolor::reset << (preserve_config_file ? "true" : "false");
}

void
setup_variables()
{
    CONFIG_PATH      = fima::fs::get_application_config_path();
    FIMA_CONFIG_PATH = CONFIG_PATH / "fima";
    MAPPINGS_PATH    = FIMA_CONFIG_PATH / "mappings";

    MAP_LANGUAGES_NAME_PATH   = MAPPINGS_PATH / "language_name.json";
    MAP_LANGUAGES_FAMILY_PATH = MAPPINGS_PATH / "language_family.json";
    MAP_LANGUAGES_ICON_PATH   = MAPPINGS_PATH / "lanugage_icon.json";
    MAP_DIRECTORY_ICON_PATH   = MAPPINGS_PATH / "directory_icon.json";
    FAMILY_SPEC_PATH          = FIMA_CONFIG_PATH / "family_spec.json";
    CONFIG_FILE_PATH          = FIMA_CONFIG_PATH / "config.toml";

    create_config_files();

    map_language_name_json    = parse_file(MAP_LANGUAGES_NAME_PATH);
    map_language_icon_json    = parse_file(MAP_LANGUAGES_ICON_PATH);
    map_languages_family_json = parse_file(MAP_LANGUAGES_FAMILY_PATH);
    map_directory_icon_json   = parse_file(MAP_DIRECTORY_ICON_PATH);
    language_file_json        = parse_file(FAMILY_SPEC_PATH);
}

} // namespace program_files

} // namespace fima
