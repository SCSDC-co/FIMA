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

#include "git/GitRepo.h"
#include "options.h"

namespace fima {

namespace info {

namespace git {

void
info(const fima::options::info_options& options, const fima::git::GitRepo& repo);

}

} // namespace info

} // namespace fima
