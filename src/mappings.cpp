/*
 * src/mappings.cpp
 * include/mappings.h
 *
 * A utility to work with the program mappings (like icon to to name).
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "mappings.h"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <regex>
#include <string_view>
#include <unordered_set>

namespace fima {

namespace mappings {

[[nodiscard]] Comments
get_language_comments(const std::filesystem::path& path)
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
        return {};
    }

    if (special_shell_type.contains(filename)) {
        return { "#" };
    }

    if (special_lua_type.contains(filename)) {
        return { "--", "--[[", "--]]" };
    }

    std::string extension = path.extension();

    auto family{ map_extension_language_family.find(extension) };

    if (family == map_extension_language_family.end()) {
        return {}; // the family doesn't exists, so we default to the text comments (no comments)
    }

    return map_language_family_comments.at(family->second);
}

[[nodiscard]] std::string_view
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

    std::string extension = path.extension().string();

    auto name{ map_extension_name.find(extension) };

    if (name == map_extension_name.end()) {
        return "Text";
    }

    return map_extension_name.at(extension);
}

[[nodiscard]] std::string_view
get_item_icon(const std::filesystem::path& path)
{
    if (std::filesystem::is_symlink(path)) {
        return "";
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

        auto icon{ map_extension_icon.find(extension) };

        if (icon == map_extension_icon.end()) {
            return "";
        }

        return map_extension_icon.at(extension);
    }

    if (std::filesystem::is_empty(path)) {
        return "";
    }

    auto icon{ map_directory_icon.find(file_name) };

    if (icon == map_directory_icon.end()) {
        return "";
    }

    return map_directory_icon.at(file_name);
}

} // namespace mappings

} // namespace fima
