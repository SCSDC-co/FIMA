/*
 * src/commands/copy/directory.cpp
 * include/commands/copy/directory.h
 *
 * The declaration of the `copy` subcommand when copying directories
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace copy {

void
directory(std::filesystem::path source, std::filesystem::path destination);

}

} // namespace fima
