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

#include <filesystem>

namespace fima {

namespace info {

void
info(const std::filesystem::directory_entry& path);

}

} // namespace fima
