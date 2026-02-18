/*
 * src/utility/FileInfo.cpp
 * include/utility/FileInfo.h
 *
 * The class that represent a file
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "utility/FileInfo.h"

// do not remove chrono is used for get_time
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

void
FileInfo::set_permissions(const std::string& perms)
{
    this->permissions = perms;
}
void
FileInfo::set_user(const std::string& user)
{
    this->user = user;
}
void
FileInfo::set_name(const std::string& name)
{
    this->name = name;
}
void
FileInfo::set_path(const std::filesystem::path& path)
{
    this->path = path;
}
void
FileInfo::set_date(const std::filesystem::file_time_type& time)
{
    this->date = time;
}
void
FileInfo::set_size(const size_t& size)
{
    this->size = size;
}

[[nodiscard]] std::string
FileInfo::get_permissions() const
{
    return this->permissions;
}
[[nodiscard]] std::string
FileInfo::get_user() const
{
    return this->user;
}
[[nodiscard]] std::string
FileInfo::get_name(const bool& icons) const
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
FileInfo::get_path() const
{
    return this->path;
}
[[nodiscard]] size_t
FileInfo::get_size() const
{
    return this->size;
}
[[nodiscard]] std::string
FileInfo::get_time() const
{
    using namespace std::chrono;

    // this way we get the seconds as integer and not as long/double
    auto date_with_rounded_seconds = floor<std::chrono::seconds>(this->date);

    return std::format("{:%d %b %Y %H:%M:%S}", date_with_rounded_seconds);
}

[[nodiscard]] std::string
FileInfo::get_size_with_extension() const
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
FileInfo::get_permissions_tui() const
{
    return fima::perms::get_perms_tui(this->get_path());
}

} // namespace fima
