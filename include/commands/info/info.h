/*
 * src/commands/info.cpp
 * include/commands/info.h
 *
 * A command that gives information about a file/directory
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

void
info(const fima::options::info_options& options, fima::git::GitRepo repo);

}

} // namespace fima
