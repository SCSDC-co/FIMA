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

#include <filesystem>
#include <iostream>

#include "fs/File.h"

namespace fima {

namespace info {

namespace file {

void
get_info(const std::filesystem::directory_entry& path)
{
    fima::fs::File file(path);

    std::cout << "path: " << file.metadata.get_path().string() << '\n';
    std::cout << "loc:" << '\n';
    std::cout << " code: " << file.stats.get_code() << '\n';
    std::cout << " comments: " << file.stats.get_comments() << '\n';
    std::cout << " blank lines: " << file.stats.get_blank_lines() << '\n';
    std::cout << " total: " << file.stats.get_total() << '\n';
    std::cout << "perms: " << file.metadata.get_permissions() << '\n';
    std::cout << "size: " << file.metadata.get_size_with_extension() << '\n';
    std::cout << "modification date: " << file.metadata.get_last_modification_date() << '\n';
}

} // namespace file

} // namespace info

} // namespace fima
