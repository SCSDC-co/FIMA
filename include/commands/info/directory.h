/*
 * src/commands/info/directory/info.cpp
 * include/commands/info/directory/info.h
 *
 * The logic for getting and printing info about a directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <filesystem>

#include "git/GitRepo.h"

namespace fima {

namespace info {

void
dir(const std::filesystem::directory_entry& path,
    const bool& verbose,
    const fima::git::GitRepo& repo);

} // namespace info

} // namespace fima
