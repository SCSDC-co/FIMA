/*
 * src/commands/create/file.cpp
 * include/commands/create/file.h
 *
 * The declaration of the `create ... file` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <vector>

namespace fima {

namespace commands {

namespace create {

void
file(const std::vector<std::filesystem::path>& paths);

}

} // namespace commands

} // namespace fima
