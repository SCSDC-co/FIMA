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

#pragma once

#include <ctime>
#include <filesystem>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

namespace fima {

namespace fs {

class DirectoryItem
{
  public:
    enum class ItemColor
    {
        RED,
        GREEN,
        WHITE,
        YELLOW,
        BLUE,
        CYAN,
    };

    DirectoryItem(const std::filesystem::directory_entry& path);

    void set_color();
    void set_size();

    [[nodiscard]] std::string get_permissions() const;
    [[nodiscard]] std::string get_owner() const;
    [[nodiscard]] std::string get_name(const bool& icons) const;
    [[nodiscard]] std::string get_last_modification_date() const;
    [[nodiscard]] std::string get_icon() const;
    [[nodiscard]] std::string get_size_with_extension() const;

    [[nodiscard]] size_t get_size() const;

    [[nodiscard]] bool is_directory() const;
    [[nodiscard]] bool is_file() const;
    [[nodiscard]] bool is_symlink() const;
    [[nodiscard]] bool is_valid() const;
    [[nodiscard]] bool get_is_hidden() const;

    [[nodiscard]] std::filesystem::path get_path() const;
    [[nodiscard]] std::filesystem::path get_symlink_target() const;

    [[nodiscard]] ftxui::Element get_permissions_tui() const;
    [[nodiscard]] ftxui::Color get_color_tui() const;

    [[nodiscard]] DirectoryItem::ItemColor get_color() const;

  private:
    bool is_hidden{};

    std::string permissions{};
    std::string owner{};
    std::string name{};
    std::string icon{};
    ItemColor color{};

    std::filesystem::path path{};
    std::filesystem::file_time_type last_modification_date{};

    size_t size{};

    ftxui::Color color_tui{};
};

} // namespace fs

} // namespace fima
