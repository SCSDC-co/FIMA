/*
 * src/tui/tui.cpp
 * include/tui/tui.h
 *
 * This file is the declaration of the main entry point of the FIMA's TUI
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace tui {

void
start_tui(std::filesystem::path path);

}

}
