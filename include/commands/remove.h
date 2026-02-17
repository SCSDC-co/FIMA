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

#include <filesystem>
#include <vector>

namespace fima {

void
remove(const std::vector<std::filesystem::path>& paths);

} // namespace fima
