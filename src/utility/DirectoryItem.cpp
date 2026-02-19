/*
 * src/utility/DirectoryItem.cpp
 * include/utility/DirectoryItem.h
 *
 * The class that represent a directory item
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "utility/DirectoryItem.h"

#include <chrono>
#include <cmath>
#include <filesystem>
#include <format>
#include <ftxui/dom/node.hpp>
#include <string>

#include "commands/ls/helpers/icon_maps.h"
#include "commands/permissions.h"
#include "ftxui/dom/elements.hpp"

namespace fima {

[[nodiscard]] std::string
DirectoryItem::get_permissions() const
{
    return this->permissions;
}
[[nodiscard]] std::string
DirectoryItem::get_user() const
{
    return this->user;
}
[[nodiscard]] std::string
DirectoryItem::get_name(const bool& icons) const
{
    std::string name;

    if (icons) {
        name += fima::ls::helpers::get_item_icon(this->get_path()) + " ";
    }

    name += this->name;

    if (std::filesystem::is_directory(this->get_path())) {
        name += "/";
    }

    return name;
}
[[nodiscard]] std::filesystem::path
DirectoryItem::get_path() const
{
    return this->path;
}
[[nodiscard]] size_t
DirectoryItem::get_size() const
{
    return this->size;
}
[[nodiscard]] std::string
DirectoryItem::get_time() const
{
    using namespace std::chrono;

    // this way we get the seconds as integer and not as long/double
    auto date_with_rounded_seconds = floor<std::chrono::seconds>(this->date);

    // since std::filesystem::last_write_time returns the time as UTC+00:00 this convert it to the
    // correct locale
    auto sctp  = std::chrono::clock_cast<std::chrono::system_clock>(date_with_rounded_seconds);
    auto local = std::chrono::zoned_time{ std::chrono::current_zone(), sctp };

    return std::format("{:%d %b %Y %H:%M:%S}", local);
}

[[nodiscard]] std::string
DirectoryItem::get_size_with_extension() const
{
    double s = static_cast<double>(this->get_size());
    std::string ext;

    if (s >= 1024 * 1024 * 1024) {
        ext = "GB";
        s /= 1024 * 1024 * 1024;
    } else if (s >= 1024 * 1024) {
        ext = "MB";
        s /= 1024 * 1024;
    } else if (s >= 1024) {
        ext = "KB";
        s /= 1024;
    } else {
        ext = "B";
    }

    // if the number is already rounded there's no need to display the decimal digits
    if (std::round(s) == s) {
        return std::format("{:.0f}{}", s, ext);
    }

    return std::format("{:.1f}{}", s, ext);
}

[[nodiscard]] ftxui::Element
DirectoryItem::get_permissions_tui() const
{
    return fima::perms::get_perms_tui(this->get_path());
}

[[nodiscard]] bool
DirectoryItem::is_directory() const
{
    return std::filesystem::is_directory(this->get_path());
}
[[nodiscard]] bool
DirectoryItem::is_file() const
{
    return !std::filesystem::is_directory(this->get_path());
}

} // namespace fima
