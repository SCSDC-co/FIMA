/*
 * include/config.h
 *
 * This file contains the variables that should be available in all the program
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <regex>
#include <string_view>
#include <vector>

namespace fima {

namespace config {

constexpr std::string_view VERSION = "1.0.0";
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
    std::regex{ R"(\.git)" },        std::regex{ R"(\.cache)" }, std::regex{ R"(\.vscode)" },
    std::regex{ R"(build)" },        std::regex{ R"(dist)" },    std::regex{ R"(target)" },
    std::regex{ R"(node_modules)" }, std::regex{ R"(\.next)" },  std::regex{ R"(bin)" },
    std::regex{ R"(obj)" }
};

inline int depth{};

void
parse_config_file();

} // namespace config

} // namespace fima
