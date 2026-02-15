/*
 * src/commands/create/file.cpp
 * include/commands/create/file.h
 *
 * The declaration of the `create ... file` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>
#include <vector>

namespace fima {

namespace create {

void
file(const std::vector<std::filesystem::path>& paths);

}

} // namespace fima
