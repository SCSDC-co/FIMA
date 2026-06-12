/*
 * src/fs/File.cpp
 * include/fs/File.h
 *
 * A class that represent a directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

#include "commands/cloc/helpers/Stats.h"
#include "fs/DirectoryItem.h"
#include "git/GitRepo.h"

namespace fima {

namespace fs {

class Directory
{
  private:
    int number_of_files{ 0 };

  public:
    fima::fs::DirectoryItem metadata;

    fima::cloc::classes::Stats stats;

    Directory(const std::filesystem::directory_entry& path);

    void set_number_of_files(const fima::git::GitRepo& repo);
    void set_stats(const fima::git::GitRepo& repo);

    [[nodiscard]] int get_number_of_files() const;
};

} // namespace fs

} // namespace fima
