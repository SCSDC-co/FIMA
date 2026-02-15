/*
 * src/commands/cloc/helpers/count_lines.cpp
 * include/commands/cloc/helpers/count_lines.h
 *
 * Declaration of the line counter of `cloc`
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>

#include "FileStats.h"

namespace fima {

namespace cloc {

namespace helpers {

fima::cloc::classes::FileStats
count_lines(const std::filesystem::path& file_path,
            const std::string& single_comment,
            const std::string& multiline_start,
            const std::string& multiline_end);

}

}

}
