/*
 * src/commands/ls.cpp
 * include/commands/ls.h
 *
 * The declaration of the `ls` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace ls {

void
start(const std::filesystem::path& path);

}

} // namespace fima
