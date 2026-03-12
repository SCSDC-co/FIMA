/*
 * src/commands/rename.cpp
 * include/commands/rename.h
 *
 * The declaration of the `rename` subcommand
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
setup_rename(CLI::App& app, std::filesystem::path& old_name, std::filesystem::path& new_name);

}

} // namespace fima
