/*
 * include/utility/colors.h
 *
 * This file contain some variables for styling strings
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <regex>
#include <string>

namespace fima {

namespace colors {

inline constexpr std::string RESET = "\033[0m";

inline constexpr std::string GREEN  = "\033[32m";
inline constexpr std::string RED    = "\033[31m";
inline constexpr std::string BLUE   = "\033[34m";
inline constexpr std::string YELLOW = "\033[33m";

inline constexpr std::string BOLD      = "\033[1m";
inline constexpr std::string UNDERLINE = "\033[4m";

inline const std::regex ESCAPE_SEQUENCE_REGEX("\033\[[0-9;]*m");

} // namespace colors

} // namespace fima
