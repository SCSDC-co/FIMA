/*
 * src/commands/trash.cpp
 * include/commands/trash.h
 *
 * The declaration of the `trash` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <CLI/App.hpp>

namespace fima {

namespace commands {

void
setup_trash(CLI::App& app, bool& yes);

}

} // namespace fima
