/*
 * src/commands/trash/restore.cpp
 * include/commands/trash/restore.h
 *
 * The declaration of the `trash restore` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <string>
#include <vector>

namespace fima {

namespace commands {

namespace trash {

void
restore(const std::vector<std::string>& ids);

}

} // namespace commands

} // namespace fima
