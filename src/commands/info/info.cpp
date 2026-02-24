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

#include "commands/info/info.h"

#include <filesystem>

#include "commands/info/directory/info.h"
#include "commands/info/file/info.h"

namespace fs = std::filesystem;

namespace fima {

namespace info {

void
info(const fs::directory_entry& path)
{
    if (path.is_directory()) {
        dir::get_info(path);
    } else {
        file::get_info(path);
    }
}

} // namespace info

} // namespace fima
