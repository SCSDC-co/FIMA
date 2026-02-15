/*
 * src/commands/tui.cpp
 * include/commands/tui.h
 *
 * The declaration of the `tree` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>
#include <string>

namespace fima {

namespace tree {

void
start(const std::filesystem::path& path, std::string prefix, bool tui);

}

} // namespace fima
