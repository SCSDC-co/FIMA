/*
 * src/commands/cloc/helpers/language_map.cpp
 * include/commands/cloc/helpers/language_map.h
 *
 * This file has the logic for the language map
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/cloc/helpers/language_map.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_set>

#include "program_files.h"

namespace _fs = std::filesystem;

namespace fima {

namespace cloc {

namespace helpers {

using json = nlohmann::json;

[[nodiscard]] std::string
get_language_family(_fs::path path)
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

    json file;

    std::ifstream file_stream(fima::program_files::MAP_LANGUAGES_FAMILY_PATH);
    file = json::parse(file_stream);

    return file.value(extension, "text");
}

[[nodiscard]] std::string
get_language_name(_fs::path path)
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

    json file;

    std::ifstream file_stream(fima::program_files::MAP_LANGUAGES_NAME_PATH);
    file = json::parse(file_stream);

    return file.value(extension, "Text");
}

} // namespace helpers

} // namespace cloc

} // namespace fima
