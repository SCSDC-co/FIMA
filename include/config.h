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

#pragma once

#include <filesystem>
#include <regex>
#include <string_view>
#include <vector>

namespace fima {

namespace config {

constexpr std::string_view VERSION = VERSION_STRING;
constexpr std::string_view LOGO    = R"(
.----.______       _____ ___ __  __    _
|           |     |  ___|_ _|  \/  |  / \
|   ___________   | |_   | || |\/| | / _ \
|  /          /   |  _|  | || |  | |/ ___ \
| /          /    |_|   |___|_|  |_/_/   \_\
|/__________/
)";

// since these are useless to analyze we put them as default to ignore
inline const std::vector<std::regex> DEFAULT_DIRS_TO_IGNORE = {
    std::regex{ R"(\.git)" },        std::regex{ R"(\.cache)" },    std::regex{ R"(\.vscode)" },
    std::regex{ R"(build)" },        std::regex{ R"(dist)" },       std::regex{ R"(target)" },
    std::regex{ R"(node_modules)" }, std::regex{ R"(\.next)" },     std::regex{ R"(bin)" },
    std::regex{ R"(obj)" },          std::regex{ R"(__pycache__)" }
};

inline int depth{ 8 };
inline bool process_directory_size{ false };

inline std::filesystem::path CONFIG_PATH;
inline std::filesystem::path FIMA_CONFIG_PATH;
inline std::filesystem::path CONFIG_FILE_PATH;
inline std::filesystem::path THEME_FILE_PATH;

void
setup_variables();

void
parse_config_file();

} // namespace config

} // namespace fima
