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

#include <regex>
#include <vector>

#include "git/GitRepo.h"
#include "options.h"

namespace fima {

namespace cloc {

void
main(const std::vector<std::regex>& paths_to_ignore,
     const fima::git::GitRepo& repo,
     const fima::options::cloc_options options);

}

} // namespace fima
