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

#include <array>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <format>
#include <fstream>
#include <ftxui/dom/node.hpp>
#include <grp.h>
#include <ios>
#include <iostream>
#include <iterator>
#include <pwd.h>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_set>
#include <vector>

#include "config.h"
#include "ftxui/dom/elements.hpp"
#include "mappings.h"
#include "theme.h"

namespace fima {

namespace fs {

namespace operations {

size_t
get_item_size(const std::filesystem::path& path)
{
    size_t size{};

    if (std::filesystem::is_directory(path)) {
        if (fima::config::process_directory_size) {
            for (auto i = std::filesystem::recursive_directory_iterator(
                   path, std::filesystem::directory_options::skip_permission_denied);
                 i != std::filesystem::recursive_directory_iterator();
                 ++i) {
                const std::filesystem::path item = i->path();

                if (i.depth() >= fima::config::depth && fima::config::depth >= 0) {
                    continue;
                }

                if (std::filesystem::is_directory(item)) {
                    continue;
                }

                if (!std::filesystem::is_regular_file(item)) {
                    continue;
                }

                if (std::filesystem::is_symlink(item) &&
                    !std::filesystem::exists(std::filesystem::read_symlink(item))) {
                    continue;
                }

                try {
                    size += std::filesystem::file_size(item);
                } catch (...) {
                    // ignores inaccessible files
                }
            }
        } else {
            // instead of the directory size we just get the number of entries
            size =
              std::distance(std::filesystem::recursive_directory_iterator(
                              path, std::filesystem::directory_options::skip_permission_denied),
                            std::filesystem::recursive_directory_iterator());
        }
    } else {
        try {
            size = std::filesystem::file_size(path);
        } catch (...) {
            // ignores inaccessible files
        }
    }

    return size;
}

std::string
make_size_readable(const size_t size)
{
    if (size == 0) {
        return "-";
    }

    std::array<char, 7> sizes{ 'B', 'K', 'M', 'G', 'T', 'P', 'E' };

    int i{};
    double mantissa = size;

    for (; mantissa >= 1000.0; mantissa /= 1000.0, ++i) {
    }

    mantissa = std::round(mantissa * 10.0) / 10.0;

    std::format_string<double&, std::string&> format_string{ "{:0.1f} {:<2}" };

    std::string extension{ std::string(1, sizes[i]) + (i ? "B" : "") };

    if (static_cast<int>(mantissa * 10.0) % 10 == 0) {
        format_string = "{:0.0f} {:<2}";
    }

    return std::format(format_string, mantissa, extension);
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

std::string
get_file_type(const std::filesystem::path& path)
{
    std::string type{};

    std::string ext{ path.extension().string() };
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (is_file_executable(path)) {
        type = "Executable";
    } else if (is_media(path)) {
        type = "Media";
    } else if (is_compressed_archive(path)) {
        type = "Compressed Archive";
    } else if (auto it = fima::mappings::map_extension_name.find(ext);
               it != fima::mappings::map_extension_name.end()) {
        type = it->second;
    } else {
        type = "Normal File";
    }

    return type;
}

bool
is_file_executable(const std::filesystem::path& path)
{
    if (std::filesystem::is_directory(path)) {
        return false;
    }

    static const std::unordered_set<std::string> ft = {
        ".exe", ".dll",   ".sys", ".cpl", ".ocx",      ".scr", ".efi",  ".msi",
        ".app", ".apk",   ".ipa", ".elf", ".o",        ".obj", ".com",  ".bin",
        ".so",  ".dylib", ".out", ".run", ".appimage", ".ko",  ".wasm", ".pyc",
    };

    static const std::unordered_set<std::string> scripts = {
        ".sh",  ".zsh", ".bash", ".csh", ".ksh", ".tcsh", ".fish",        ".py", ".pyw",
        ".pl",  ".pm",  ".rb",   ".php", ".js",  ".mjs",  ".cjs",         ".ts", ".lua",
        ".ps1", ".vbs", ".bat",  ".cmd", ".awk", ".tcl",  ".applescript",
    };

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    // scripts are text even when executable
    if (scripts.contains(ext)) {
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
        ".zip",  ".rar", ".7z",   ".tar",   ".gz",  ".bz2",  ".xz",  ".z",   ".lz",
        ".lzma", ".lzo", ".zst",  ".dmg",   ".pkg", ".xip",  ".cab", ".msi", ".wim",
        ".deb",  ".rpm", ".snap", ".jar",   ".war", ".ear",  ".aar", ".apk", ".ipa",
        ".whl",  ".egg", ".tgz",  ".tbz2",  ".txz", ".tzst", ".lz4", ".br",  ".iso",
        ".img",  ".ar",  ".cpio", ".crate", ".ace", ".arc",  ".arj",
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
        ".dff",  ".caf",  ".pcm", ".jxl", ".jfif", ".jpe",  ".svg",  ".svgz", ".tga",  ".exr",
        ".hdr",  ".bpg",  ".qoi", ".pbm", ".pgm",  ".ppm",  ".pnm",  ".mts",  ".m2v",  ".ts",
        ".ogm",  ".y4m",  ".m4b", ".m4p", ".m4r",  ".weba", ".spx",  ".au",   ".snd",  ".voc",
        ".w64",  ".mpc",  ".shn", ".tak", ".xm",   ".mod",  ".s3m",  ".it",   ".aifc", ".mp1",
        ".mp2",
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

bool
is_ancestor(const std::filesystem::path& ancestor, const std::filesystem::path& descendant)
{
    const std::filesystem::path a = std::filesystem::weakly_canonical(ancestor);
    const std::filesystem::path d = std::filesystem::weakly_canonical(descendant);

    auto rel = d.lexically_relative(a);

    return !rel.empty() && !rel.string().starts_with("..");
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
        std::cerr << fima::theme::theme.error
                  << "Failed to get permissions for: " << fima::theme::theme.secondary
                  << item.string() << fima::theme::Color::reset << '\n';

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

        auto show = [=](char op, std::filesystem::perms perm) {
            Element element;

            Color _color{};

            switch (op) {
                case 'r':
                    _color = fima::theme::theme.perms_read.get_color_for_tui();
                    break;
                case 'w':
                    _color = fima::theme::theme.perms_write.get_color_for_tui();
                    break;
                case 'x':
                    _color = fima::theme::theme.perms_exec.get_color_for_tui();
                    break;
            }

            if (std::filesystem::perms::none == (perm & permissions)) {
                element = text("-") | color(fima::theme::theme.perms_null.get_color_for_tui());
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
            element_vector.push_back(text("d") |
                                     color(fima::theme::theme.directory.get_color_for_tui()));
        } else if (std::filesystem::is_symlink(st)) {
            element_vector.push_back(text("l") |
                                     color(fima::theme::theme.symlink.get_color_for_tui()));
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

        element_vector.push_back(show('r', std::filesystem::perms::owner_read));
        element_vector.push_back(show('w', std::filesystem::perms::owner_write));
        element_vector.push_back(show('x', std::filesystem::perms::owner_exec));
        element_vector.push_back(show('r', std::filesystem::perms::group_read));
        element_vector.push_back(show('w', std::filesystem::perms::group_write));
        element_vector.push_back(show('x', std::filesystem::perms::group_exec));
        element_vector.push_back(show('r', std::filesystem::perms::others_read));
        element_vector.push_back(show('w', std::filesystem::perms::others_write));
        element_vector.push_back(show('x', std::filesystem::perms::others_exec));
    } catch (const std::exception& ex) {
        std::cerr << fima::theme::theme.error
                  << "Failed to get permissions for: " << fima::theme::theme.secondary
                  << item.string() << fima::theme::Color::reset << '\n';

        std::cerr << ex.what();
    }

    Element perms = hbox(element_vector);

    return perms;
}

} // namespace operations

} // namespace fs

} // namespace fima
