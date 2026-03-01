/*
 * src/fs/DirectoryItem.cpp
 * include/fs/DirectoryItem.h
 *
 * The class that represent a directory item
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/DirectoryItem.h"

#include <chrono>
#include <cmath>
#include <filesystem>
#include <format>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

#include "commands/permissions.h"
#include "fs/operations.h"
#include "ftxui/dom/elements.hpp"
#include "program_files.h"
#include "utility/colors.h"

namespace fima {

namespace fs {

DirectoryItem::DirectoryItem(const std::filesystem::directory_entry& path)
  : name(path.path().filename().string())
  , path(path)
  , permissions(fima::perms::get_perms(path))
  , last_modification_date(fima::fs::operations::get_file_time(path))
  , owner(fima::fs::operations::get_file_owner(path))
  , is_hidden(path.path().filename().native().starts_with('.'))
  , icon(fima::program_files::get_item_icon(path.path()) + " ")
{
    this->set_color();
}

void
DirectoryItem::set_color()
{
    std::string color;

    if (this->is_directory()) {
        color = fima::colors::GREEN;
    } else if (this->is_symlink()) {
        color = fima::colors::CYAN;
    } else if (fima::fs::operations::is_file_executable(this->path)) {
        color = fima::colors::RED;
    } else if (fima::fs::operations::is_compressed_archive(this->path)) {
        color = fima::colors::BLUE;
    } else if (fima::fs::operations::is_media(this->path)) {
        color = fima::colors::YELLOW;
    } else {
        color = fima::colors::WHITE;
    }

    if (color == fima::colors::RED) {
        this->color_tui = ftxui::Color::Red;
    } else if (color == fima::colors::GREEN) {
        this->color_tui = ftxui::Color::Green;
    } else if (color == fima::colors::WHITE) {
        this->color_tui = ftxui::Color::White;
    } else if (color == fima::colors::YELLOW) {
        this->color_tui = ftxui::Color::Yellow;
    } else if (color == fima::colors::BLUE) {
        this->color_tui = ftxui::Color::Blue;
    } else if (color == fima::colors::CYAN) {
        this->color_tui = ftxui::Color::Cyan;
    }

    this->color = color;
}

void
DirectoryItem::set_size()
{
    this->size = fima::fs::operations::get_item_size(this->path);
}

[[nodiscard]] std::string
DirectoryItem::get_permissions() const
{
    return this->permissions;
}
[[nodiscard]] std::string
DirectoryItem::get_owner() const
{
    return this->owner;
}
[[nodiscard]] std::string
DirectoryItem::get_name(const bool& icons) const
{
    std::string name;

    if (icons) {
        if (this->is_symlink()) {
            name += " ";
        } else {
            name += fima::program_files::get_item_icon(this->get_path()) + " ";
        }
    }

    name += this->name;

    if (this->is_directory()) {
        name += "/";
    }

    return name;
}
[[nodiscard]] std::filesystem::path
DirectoryItem::get_path() const
{
    return this->path;
}
[[nodiscard]] std::filesystem::path
DirectoryItem::get_symlink_target() const
{
    return std::filesystem::read_symlink(this->get_path());
}
[[nodiscard]] size_t
DirectoryItem::get_size() const
{
    return this->size;
}
[[nodiscard]] std::string
DirectoryItem::get_last_modification_date() const
{
    if (this->last_modification_date == std::filesystem::file_time_type()) {
        return "unknown";
    }

    using namespace std::chrono;

    // this way we get the seconds as integer and not as long/double
    auto date_with_rounded_seconds = floor<std::chrono::seconds>(this->last_modification_date);

    // since std::filesystem::last_write_time returns the time as UTC+00:00 this convert it to the
    // correct locale
    auto sctp  = std::chrono::clock_cast<std::chrono::system_clock>(date_with_rounded_seconds);
    auto local = std::chrono::zoned_time{ std::chrono::current_zone(), sctp };

    return std::format("{:%d %b %Y %H:%M:%S}", local);
}
[[nodiscard]] std::string
DirectoryItem::get_icon() const
{
    return this->icon;
}
[[nodiscard]] std::string
DirectoryItem::get_size_with_extension() const
{
    if (this->get_size() == 0) {
        return "-";
    }

    double dimension = static_cast<double>(this->get_size());
    std::string ext;

    if (dimension >= 1024 * 1024 * 1024) {
        ext = "GB";
        dimension /= 1024 * 1024 * 1024;
    } else if (dimension >= 1024 * 1024) {
        ext = "MB";
        dimension /= 1024 * 1024;
    } else if (dimension >= 1024) {
        ext = "KB";
        dimension /= 1024;
    } else {
        ext = "B";
    }

    // if the number is already rounded there's no need to display the decimal digits
    if (std::round(dimension) == static_cast<int>(dimension)) {
        return std::format("{:.0f}{}", dimension, ext);
    }

    return std::format("{:.2f}{}", dimension, ext);
}
[[nodiscard]] std::string
DirectoryItem::get_color() const
{
    return this->color;
}

[[nodiscard]] ftxui::Element
DirectoryItem::get_permissions_tui() const
{
    return fima::perms::get_perms_tui(this->get_path());
}

[[nodiscard]] ftxui::Color
DirectoryItem::get_color_tui() const
{
    return this->color_tui;
}

[[nodiscard]] bool
DirectoryItem::is_directory() const
{
    return std::filesystem::is_directory(this->get_path());
}
[[nodiscard]] bool
DirectoryItem::is_file() const
{
    return !std::filesystem::is_directory(this->get_path()) &&
           !std::filesystem::is_symlink(this->get_path());
}
[[nodiscard]] bool
DirectoryItem::is_symlink() const
{
    return std::filesystem::is_symlink(this->get_path());
}
[[nodiscard]] bool
DirectoryItem::is_valid() const
{
    return this->get_last_modification_date() == "unknown" && this->get_size() == 0 &&
           this->get_owner() == "unknown";
}
[[nodiscard]] bool
DirectoryItem::get_is_hidden() const
{
    return this->is_hidden;
}

} // namespace fs

} // namespace fima
