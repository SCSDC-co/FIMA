/*
 * src/utility/file.cpp
 * include/utility/file.h
 *
 * A utility for getting some file information and creating them
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

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

#include "helpers/get_directories_entries.h"

namespace fs = std::filesystem;

namespace fima {

namespace file {

size_t
get_file_size(const fs::path& path)
{
    size_t size{};

    if (fs::is_directory(path)) {
        std::vector<fs::path> entries =
          fima::helpers::get_directories_entries_recursive(path, false, {});

        for (const fs::path& item : entries) {
            if (fs::is_directory(item)) {
                continue;
            }

            size = fs::file_size(item);
        }
    } else {
        size = fs::file_size(path);
    }

    return size;
}

fs::file_time_type
get_file_time(const fs::path& path)
{
    return fs::last_write_time(path);
}

std::string
get_file_owner(const fs::path& path)
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
overwrite(const fs::path& path, const std::string_view& content)
{
    std::ofstream file{ path, std::ios::trunc };

    file << content;

    file.close();
}

} // namespace file

} // namespace fima
