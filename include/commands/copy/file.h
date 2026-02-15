/*
 * src/commands/copy/file.cpp
 * include/commands/copy/file.h
 *
 * The declaration of the `copy` subcommand when copying files
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace copy {

void
file(std::filesystem::path source, std::filesystem::path destination);

}

} // namespace fima
