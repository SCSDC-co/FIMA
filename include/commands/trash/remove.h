/*
 * src/commands/trash/remove.cpp
 * include/commands/trash/remove.h
 *
 * The declaration of the `trash remove` subcommand
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
remove(const std::vector<std::string>& ids);

}

} // namespace commands

} // namespace fima
