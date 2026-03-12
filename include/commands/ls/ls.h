/*
 * src/commands/ls.cpp
 * include/commands/ls.h
 *
 * The declaration of the `ls` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

#include "cli/CLI11.hpp"
#include "git/GitRepo.h"
#include "options.h"

namespace fima {

namespace commands {

void
setup_ls(CLI::App& app,
         const std::filesystem::directory_entry& path,
         const fima::git::GitRepo& repo,
         fima::options::ls_options& options);

} // namespace commands

} // namespace fima
