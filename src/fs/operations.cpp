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
#include <ftxui/dom/node.hpp>
#include <grp.h>
#include <ios>
#include <iostream>
#include <pwd.h>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <termcolor/termcolor.hpp>
#include <unistd.h>
#include <unordered_set>

#include "config.h"
#include "ftxui/dom/elements.hpp"

namespace fima {

namespace fs {

namespace operations {

size_t
get_item_size(const std::filesystem::path& path)
{
    size_t size{};

    if (std::filesystem::is_directory(path)) {
        for (auto i = std::filesystem::recursive_directory_iterator(
               path, std::filesystem::directory_options::skip_permission_denied);
             i != std::filesystem::recursive_directory_iterator();
             ++i) {
            if (i.depth() >= fima::config::depth && fima::config::depth >= 0) {
                break;
            }

            if (std::filesystem::is_directory(i->path())) {
                continue;
            }

            if (!std::filesystem::is_regular_file(i->path())) {
                continue;
            }

            if (std::filesystem::is_symlink(i->path()) &&
                !std::filesystem::exists(std::filesystem::read_symlink(i->path()))) {
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
    try {
        return std::filesystem::last_write_time(path);
    } catch (...) {
        return std::filesystem::file_time_type();
    }
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
    static const std::unordered_set<std::string> ft = { ".exe", ".dll", ".sys", ".cpl",
                                                        ".ocx", ".scr", ".efi", ".msi",
                                                        ".app", ".apk", ".ipa", ".elf" };

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".sh" || ext == ".zsh") {
        return false;
    }

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

    return ft.contains(ext);
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

    return ft.contains(ext);
}

bool
is_root(const std::filesystem::path& path)
{
    std::filesystem::path canonical_path = std::filesystem::canonical(path);

    return !canonical_path.has_relative_path();
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

std::string
get_perms(const std::filesystem::path& item)
{
    std::string perms{};

    try {
        auto st = std::filesystem::status(item);

        std::filesystem::perms permissions{ st.permissions() };

        auto show = [&](char op, std::filesystem::perms perm) {
            return (std::filesystem::perms::none == (perm & permissions) ? '-' : op);
        };

        if (std::filesystem::is_regular_file(st)) {
            perms += '-';
        } else if (std::filesystem::is_directory(st)) {
            perms += 'd';
        } else if (std::filesystem::is_symlink(st)) {
            perms += 'l';
        } else if (std::filesystem::is_character_file(st)) {
            perms += 'c';
        } else if (std::filesystem::is_block_file(st)) {
            perms += 'b';
        } else if (std::filesystem::is_fifo(st)) {
            perms += 'p';
        } else if (std::filesystem::is_socket(st)) {
            perms += 's';
        } else {
            perms += '?'; // unknown type
        }

        perms += show('r', std::filesystem::perms::owner_read);
        perms += show('w', std::filesystem::perms::owner_write);
        perms += show('x', std::filesystem::perms::owner_exec);
        perms += show('r', std::filesystem::perms::group_read);
        perms += show('w', std::filesystem::perms::group_write);
        perms += show('x', std::filesystem::perms::group_exec);
        perms += show('r', std::filesystem::perms::others_read);
        perms += show('w', std::filesystem::perms::others_write);
        perms += show('x', std::filesystem::perms::others_exec);
    } catch (const std::exception& ex) {
        std::cerr << termcolor::red << "Failed to get permissions for: " << termcolor::reset
                  << item.string() << '\n';

        std::cerr << ex.what();
    }

    return perms;
}

ftxui::Element
get_perms_tui(const std::filesystem::path& item)
{
    using namespace ftxui;

    std::vector<Element> element_vector{};

    try {
        auto st = std::filesystem::status(item);

        std::filesystem::perms permissions{ st.permissions() };

        auto show = [=](char op, std::filesystem::perms perm, Color _color) {
            Element element;

            if (std::filesystem::perms::none == (perm & permissions)) {
                element = text("-") | color(Color::GrayDark);
            } else if (perm == std::filesystem::perms::owner_read ||
                       perm == std::filesystem::perms::owner_exec ||
                       perm == std::filesystem::perms::owner_write) {
                element = text(std::string(1, op)) | color(_color) | bold | underlined;
            } else {
                element = text(std::string(1, op)) | color(_color);
            }

            return element;
        };

        if (std::filesystem::is_regular_file(st)) {
            element_vector.push_back(text("-") | bold);
        } else if (std::filesystem::is_directory(st)) {
            element_vector.push_back(text("d") | color(Color::Green));
        } else if (std::filesystem::is_symlink(st)) {
            element_vector.push_back(text("l") | color(Color::Blue));
        } else if (std::filesystem::is_character_file(st)) {
            element_vector.push_back(text("c") | bold);
        } else if (std::filesystem::is_block_file(st)) {
            element_vector.push_back(text("b") | bold);
        } else if (std::filesystem::is_fifo(st)) {
            element_vector.push_back(text("p") | bold);
        } else if (std::filesystem::is_socket(st)) {
            element_vector.push_back(text("s") | bold);
        } else {
            element_vector.push_back(text("?") | bold); // unknown type
        }

        element_vector.push_back(show('r', std::filesystem::perms::owner_read, Color::Green));
        element_vector.push_back(show('w', std::filesystem::perms::owner_write, Color::Yellow));
        element_vector.push_back(show('x', std::filesystem::perms::owner_exec, Color::Red));
        element_vector.push_back(show('r', std::filesystem::perms::group_read, Color::Green));
        element_vector.push_back(show('w', std::filesystem::perms::group_write, Color::Yellow));
        element_vector.push_back(show('x', std::filesystem::perms::group_exec, Color::Red));
        element_vector.push_back(show('r', std::filesystem::perms::others_read, Color::Green));
        element_vector.push_back(show('w', std::filesystem::perms::others_write, Color::Yellow));
        element_vector.push_back(show('x', std::filesystem::perms::others_exec, Color::Red));
    } catch (const std::exception& ex) {
        std::cerr << termcolor::red << "Failed to get permissions for: " << termcolor::reset
                  << item.string() << '\n';

        std::cerr << ex.what();
    }

    Element perms = hbox(element_vector);

    return perms;
}

} // namespace operations

} // namespace fs

} // namespace fima
