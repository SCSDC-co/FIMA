/*
 * src/tui/commands/tree/ls_tui.cpp
 * include/tui/commands/tree/ls_tui.h
 *
 * This file defines the declaration of the TUI for the `ls` subcommand of FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <string>
#include <vector>

namespace fima {

namespace ls {

void
tui(std::vector<std::string> dirs_vector, std::vector<std::string> files_vector);
}

}
