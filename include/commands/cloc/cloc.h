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

#include "cli/CLI11.hpp"
#include "git/GitRepo.h"
#include "options.h"

namespace fima {

namespace commands {

void
setup_cloc(CLI::App& app, const fima::git::GitRepo& repo, fima::options::cloc_options& options);

} // namespace commands

} // namespace fima
