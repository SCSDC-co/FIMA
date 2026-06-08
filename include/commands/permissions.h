/*
 * src/commands/permissions.cpp
 * include/commands/permissions.h
 *
 * The declaration of the `permissions` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <vector>

#include "CLI/App.hpp"

namespace fima {

namespace commands {

void
setup_permissions(CLI::App& app, std::vector<std::filesystem::path>& paths);

}

} // namespace fima
