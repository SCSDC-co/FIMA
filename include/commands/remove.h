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

#include <CLI/App.hpp>

#include "options.h"

namespace fima {

namespace commands {

void
setup_remove(CLI::App& app, fima::options::rm_options& options);

}

} // namespace fima
