/*
 * src/commands/info/file.cpp
 * include/commands/info/file.h
 *
 * The logic for getting and printing info about a file
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace info {

namespace file {

void
get_info(const std::filesystem::directory_entry& path);

}

} // namespace info

} // namespace fima
