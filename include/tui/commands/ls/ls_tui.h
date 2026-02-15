/*
 * src/tui/commands/tree/ls_tui.cpp
 * include/tui/commands/tree/ls_tui.h
 *
 * This file defines the declaration of the TUI for the `ls` subcommand of FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
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
