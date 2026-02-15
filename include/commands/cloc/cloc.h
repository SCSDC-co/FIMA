/*
 * src/commands/cloc/cloc.cpp
 * include/commands/cloc/cloc.h
 *
 * The main declaration of the entry point of the `cloc` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>
#include <unordered_set>
#include <vector>

namespace fima {

namespace cloc {

void
main(const std::vector<std::filesystem::path>& paths,
     const bool& show_languages,
     const std::unordered_set<std::filesystem::path>& paths_to_ignore);

}

}
