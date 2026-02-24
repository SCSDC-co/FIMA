/*
 * src/commands/info/directory.cpp
 * include/commands/info/directory.h
 *
 * The logic for getting and printing info about a directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <filesystem>
#include <iostream>

#include "fs/Directory.h"

namespace fima {

namespace info {

namespace dir {

void
get_info(const std::filesystem::directory_entry& path)
{
    fima::fs::Directory dir(path);

    std::cout << "path: " << dir.metadata.get_path().string() << '\n';
    std::cout << "loc:" << '\n';
    std::cout << "  code: " << dir.stats.get_code() << '\n';
    std::cout << "  blank: " << dir.stats.get_blank_lines() << '\n';
    std::cout << "  comments: " << dir.stats.get_comments() << '\n';
    std::cout << "  total: " << dir.stats.get_total() << '\n';
    std::cout << "perms: " << dir.metadata.get_permissions() << '\n';
    std::cout << "size: " << dir.metadata.get_size_with_extension() << '\n';
    std::cout << "modification date: " << dir.metadata.get_last_modification_date() << '\n';
}

} // namespace dir

} // namespace info

} // namespace fima
