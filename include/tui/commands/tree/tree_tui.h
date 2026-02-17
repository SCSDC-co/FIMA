/*
 * src/tui/commands/tree/tree_tui.cpp
 * include/tui/commands/tree/tree_tui.h
 *
 * This file defines the declaration of the TUI for the `tree` subcommand of FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

using namespace ftxui;

namespace fima {

namespace tree {

void
tui(std::string path_name,
    std::vector<ftxui::Element> tree_vector_tui,
    size_t number_of_dirs,
    size_t number_of_files);

}

} // namespace fima
