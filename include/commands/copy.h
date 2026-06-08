/*
 * src/commands/copy.cpp
 * include/commands/copy.h
 *
 * The declaration of the `copy` subcommand when copying files
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include "CLI/App.hpp"

namespace fima {

namespace commands {

void
setup_copy(CLI::App& app, std::filesystem::path& source, std::filesystem::path& destination);

} // namespace commands

} // namespace fima
