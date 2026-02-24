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
#include <string>

namespace fima {

namespace fs {

class DirectoryItem
{
  private:
    bool is_hidden{};

    std::string permissions{};
    std::string user{};
    std::string name{};

    std::filesystem::path path{};
    std::filesystem::file_time_type last_modification_date{};

    size_t size{};

  public:
    DirectoryItem(const std::filesystem::directory_entry& path);

    [[nodiscard]] std::string get_permissions() const;
    [[nodiscard]] std::string get_user() const;
    [[nodiscard]] std::string get_name(const bool& icons) const;
    [[nodiscard]] std::filesystem::path get_path() const;
    [[nodiscard]] size_t get_size() const;
    [[nodiscard]] std::string get_last_modification_date() const;

    [[nodiscard]] std::string get_size_with_extension() const;
    [[nodiscard]] ftxui::Element get_permissions_tui() const;

    [[nodiscard]] bool is_directory() const;
    [[nodiscard]] bool is_file() const;
};

} // namespace fs

} // namespace fima
