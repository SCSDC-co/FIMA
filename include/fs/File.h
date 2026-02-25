/*
 * src/fs/File.cpp
 * include/fs/File.h
 *
 * A class that represent a file
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

#include "commands/cloc/helpers/Stats.h"
#include "fs/DirectoryItem.h"

namespace fima {

namespace fs {

class File
{
  private:
    std::string file_type{};

  public:
    fima::fs::DirectoryItem metadata;
    fima::cloc::classes::Stats stats;

    File(const std::filesystem::directory_entry& path);

    void set_stats();

    [[nodiscard]] std::string get_file_type() const;
};

} // namespace fs

} // namespace fima
