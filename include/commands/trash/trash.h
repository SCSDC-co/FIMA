/*
 * src/commands/trash/trash.cpp
 * include/commands/trash/trash.h
 *
 * The declaration of the `trash` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <CLI/App.hpp>
#include <vector>

namespace fima {

namespace commands {

void
setup_trash(CLI::App& app, bool& yes, std::vector<std::string>& ids, bool& list_plain);

}

} // namespace fima
