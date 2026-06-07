/*
 * src/commands/remove.cpp
 * include/commands/remove.h
 *
 * The declaration of the `remove` subcommand
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
setup_remove(CLI::App& app, std::vector<std::filesystem::path>& paths, bool& recursive);

}

} // namespace fima
