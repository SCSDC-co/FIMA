/*
 * src/commands/remove.cpp
 * include/commands/remove.h
 *
 * The declaration of the `remove` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>
#include <vector>

namespace fima {

void
remove(const std::vector<std::filesystem::path>& paths);

} // namespace fima
