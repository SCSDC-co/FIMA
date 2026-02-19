/*
 * src/commands/cloc/helpers/language_map.cpp
 * include/commands/cloc/helpers/language_map.h
 *
 * This file contains the 2 language maps:
 * extension -> family
 * extension -> name
 *
 * And the declaration for the logic of them
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <string>

namespace fima {

namespace cloc {

namespace helpers {

[[nodiscard]] std::string
get_language_family(std::filesystem::path path);

[[nodiscard]] std::string
get_language_name(std::filesystem::path path);

} // namespace helpers

} // namespace cloc

} // namespace fima
