/*
 * src/fs/get_directories_entries.cpp
 * include/fs/get_directories_entries.h
 *
 * This file contains the declaration of the helpers for getting the entries of a directory, both
 * recursively and not
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <regex>
#include <vector>

#include "git/GitRepo.h"

namespace fima {

namespace fs {

std::vector<std::filesystem::directory_entry>
get_directories_entries(const std::filesystem::path& path,
                        const fima::git::GitRepo& repo,
                        const bool& dotfiles);

std::vector<std::filesystem::directory_entry>
get_directories_entries_no_git(const std::filesystem::path& path, const bool& dotfiles);

std::vector<std::filesystem::path>
get_directories_entries_recursive(const std::filesystem::path& path,
                                  const fima::git::GitRepo& repo,
                                  const bool& ignore_directories_or_files,
                                  const std::vector<std::regex>& ignored_files_or_directories);

std::vector<std::filesystem::path>
get_directories_entries_recursive_no_git(
  const std::filesystem::path& path,
  const bool& ignore_directories_or_files,
  const std::vector<std::regex>& ignored_files_or_directories);

} // namespace fs

} // namespace fima
