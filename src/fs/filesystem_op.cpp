/*
 * src/fs/filesystem_op.cpp
 * include/fs/filesystem_op.h
 *
 * A module for doing general file and directory operations
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/filesystem_op.h"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <grp.h>
#include <ios>
#include <pwd.h>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "fs/get_directories_entries.h"

namespace fima {

namespace fs {

namespace operations {

size_t
get_file_size(const std::filesystem::path& path)
{
    size_t size{};

    if (std::filesystem::is_directory(path)) {
        std::vector<std::filesystem::path> entries =
          fima::fs::get_directories_entries_recursive(path, false, {});

        for (const std::filesystem::path& item : entries) {
            if (std::filesystem::is_directory(item)) {
                continue;
            }

            size += std::filesystem::file_size(item);
        }
    } else {
        size = std::filesystem::file_size(path);
    }

    return size;
}

std::filesystem::file_time_type
get_file_time(const std::filesystem::path& path)
{
    return std::filesystem::last_write_time(path);
}

std::string
get_file_owner(const std::filesystem::path& path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0) {
        return "unknown";
    }

    struct passwd* pw = getpwuid(info.st_uid);

    if (pw) {
        return pw->pw_name;
    }

    return std::to_string(info.st_uid);
}

void
create(const std::filesystem::path& path, const std::string_view& conent)
{
    std::ofstream outfile{ path };

    outfile << conent;

    outfile.close();
}

void
overwrite(const std::filesystem::path& path, const std::string_view& content)
{
    std::ofstream file{ path, std::ios::trunc };

    file << content;

    file.close();
}

} // namespace operations

} // namespace fs

} // namespace fima
