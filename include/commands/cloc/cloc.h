/*
 * src/commands/cloc/cloc.cpp
 * include/commands/cloc/cloc.h
 *
 * The main declaration of the entry point of the `cloc` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <regex>
#include <vector>

namespace fima {

namespace cloc {

void
main(const std::vector<std::filesystem::path>& paths,
     const bool& show_languages,
     const std::vector<std::regex>& paths_to_ignore,
     const std::string& sorting,
     const bool& quiet);

}

} // namespace fima
