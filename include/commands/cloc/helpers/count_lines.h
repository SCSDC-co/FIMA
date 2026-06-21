/*
 * src/commands/cloc/helpers/count_lines.cpp
 * include/commands/cloc/helpers/count_lines.h
 *
 * Declaration of the line counter of `cloc`
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

#include "Stats.h"
#include "mappings.h"

namespace fima {

namespace cloc {

namespace helpers {

fima::cloc::classes::Stats
count_lines(const std::filesystem::path& file_path, const fima::mappings::Comments& comments);

}

} // namespace cloc

} // namespace fima
