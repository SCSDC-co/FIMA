/*
 * src/commands/ls.cpp
 * include/commands/ls.h
 *
 * The declaration of the `ls` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace ls {

void
start(std::filesystem::path path, bool tui);

}

} // namespace fima
