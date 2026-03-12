/*
 * src/commands/create.cpp
 * include/commands/create.h
 *
 * The declaration of the `create` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include "cli/CLI11.hpp"

namespace fima {

namespace commands {

void
setup_create(CLI::App& app,
             std::vector<std::filesystem::path>& file_paths,
             std::vector<std::filesystem::path>& dir_paths);

}

} // namespace fima
