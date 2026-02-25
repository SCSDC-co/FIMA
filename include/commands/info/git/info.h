/*
 * src/commands/info/git/info.cpp
 * include/commands/info/git/info.h
 *
 * The logic for getting and printing info about a git repo
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

#include "git/GitRepo.h"

namespace fima {

namespace info {

namespace git {

void
info(const std::filesystem::directory_entry& path, const fima::git::GitRepo& repo);

}

} // namespace info

} // namespace fima
