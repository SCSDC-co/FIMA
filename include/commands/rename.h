/*
 * src/commands/rename.cpp
 * include/commands/rename.h
 *
 * The declaration of the `rename` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>

namespace fima {

void
rename(std::filesystem::path old_name, std::filesystem::path new_name);

}
