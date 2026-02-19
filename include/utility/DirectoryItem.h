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

#include "commands/permissions.h"
#include "utility/file.h"

namespace fima {

class DirectoryItem
{
  private:
    std::string permissions{};
    std::string user{};
    std::string name{};
    std::filesystem::path path{};
    std::filesystem::file_time_type date{};
    size_t size{};

  public:
    DirectoryItem(const std::filesystem::path& path)
      : name(path.filename().string())
      , path(path)
      , permissions(fima::perms::get_perms(path))
      , size(fima::file::get_file_size(path))
      , date(fima::file::get_file_time(path))
      , user(fima::file::get_file_owner(path))
    {
    }

    void set_permissions(const std::string& perms);
    void set_user(const std::string& user);
    void set_name(const std::string& name);
    void set_path(const std::filesystem::path& path);
    void set_date(const std::filesystem::file_time_type& time);
    void set_size(const size_t& size);

    [[nodiscard]] std::string get_permissions() const;
    [[nodiscard]] std::string get_user() const;
    [[nodiscard]] std::string get_name(const bool& icons) const;
    [[nodiscard]] std::filesystem::path get_path() const;
    [[nodiscard]] size_t get_size() const;
    [[nodiscard]] std::string get_time() const;

    [[nodiscard]] std::string get_size_with_extension() const;
    [[nodiscard]] ftxui::Element get_permissions_tui() const;

    [[nodiscard]] bool is_directory() const;
    [[nodiscard]] bool is_file() const;
};

} // namespace fima
