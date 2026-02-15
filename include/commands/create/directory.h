/*
 * src/commands/create/directory.cpp
 * include/commands/create/directory.h
 *
 * The declaration of the `create ... dir` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>
#include <vector>

namespace fima {

namespace create {

void
dir(const std::vector<std::filesystem::path>& paths);

}

} // namespace fima
