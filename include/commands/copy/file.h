/*
 * src/commands/copy/file.cpp
 * include/commands/copy/file.h
 *
 * The declaration of the `copy` subcommand when copying files
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace commands {

namespace copy {

void
file(std::filesystem::path source, std::filesystem::path destination);

}

} // namespace commands

} // namespace fima
