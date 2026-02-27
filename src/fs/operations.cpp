/*
 * src/fs/operations.cpp
 * include/fs/operations.h
 *
 * A module for doing general file and directory operations
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/operations.h"

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
#include <unordered_set>

#include "config.h"

namespace fima {

namespace fs {

namespace operations {

size_t
get_item_size(const std::filesystem::path& path)
{
    size_t size{};

    if (std::filesystem::is_directory(path)) {
        for (auto i = std::filesystem::recursive_directory_iterator(path);
             i != std::filesystem::recursive_directory_iterator();
             ++i) {
            if (i.depth() >= fima::config::depth) {
                break;
            }

            if (std::filesystem::is_directory(i->path())) {
                continue;
            }

            if (!std::filesystem::is_regular_file(i->path())) {
                continue;
            }

            try {
                size += std::filesystem::file_size(i->path());
            } catch (...) {
                // ignores inaccessible files
            }
        }
    } else if (!std::filesystem::is_regular_file(path)) {
        size = 0;
    } else {
        try {
            size = std::filesystem::file_size(path);
        } catch (...) {
            // ignores inaccessible files
        }
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

bool
is_file_executable(const std::filesystem::path& path)
{
    static const std::unordered_set<std::string> ft = {
        ".exe", ".dll", ".sys", ".cpl", ".ocx", ".scr", ".efi", ".msi", ".app", ".apk", ".ipa",
    };

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ft.contains(ext)) {
        return true;
    }

    auto st = std::filesystem::status(path);

    std::filesystem::perms permissions{ st.permissions() };

    return (permissions & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ||
           (permissions & std::filesystem::perms::group_exec) != std::filesystem::perms::none ||
           (permissions & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
}

bool
is_compressed_archive(const std::filesystem::path& path)
{
    static const std::unordered_set<std::string> ft = {
        ".zip",  ".rar", ".7z",  ".tar", ".gz",  ".bz2", ".xz",  ".z",   ".lz",  ".lzma",
        ".lzo",  ".zst", ".dmg", ".pkg", ".xip", ".cab", ".msi", ".wim", ".deb", ".rpm",
        ".snap", ".jar", ".war", ".ear", ".aar", ".apk", ".ipa", ".whl", ".egg",
    };

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ft.contains(ext)) {
        return true;
    } else {
        return false;
    }
}

bool
is_media(const std::filesystem::path& path)
{
    static const std::unordered_set<std::string> ft = {
        ".jpg",  ".jpeg", ".png", ".bmp", ".tiff", ".tif",  ".webp", ".avif", ".heif", ".heic",
        ".ico",  ".cur",  ".psd", ".xcf", ".raw",  ".cr2",  ".nef",  ".arw",  ".dng",  ".gif",
        ".mp4",  ".mkv",  ".avi", ".mov", ".wmv",  ".flv",  ".mpeg", ".mpg",  ".3gp",  ".3g2",
        ".m2ts", ".vob",  ".ogv", ".rm",  ".rmvb", ".asf",  ".divx", ".hevc", ".h264", ".h265",
        ".f4v",  ".mxf",  ".roq", ".drc", ".amv",  ".webm", ".m4v",  ".mp3",  ".aac",  ".m4a",
        ".ogg",  ".opus", ".wma", ".amr", ".ac3",  ".flac", ".alac", ".wav",  ".aiff", ".aif",
        ".ape",  ".wv",   ".tta", ".oga", ".mka",  ".ra",   ".mid",  ".midi", ".rmi",  ".dsf",
        ".dff",  ".caf",  ".pcm",
    };

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ft.contains(ext)) {
        return true;
    } else {
        return false;
    }
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
