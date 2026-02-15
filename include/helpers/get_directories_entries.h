/*
 * src/helpers/get_directories_entries.cpp
 * include/helpers/get_directories_entries.h
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

namespace fima {

namespace helpers {

std::vector<std::filesystem::directory_entry>
get_directories_entries(const std::filesystem::path& path);

std::vector<std::filesystem::path>
get_directories_entries_recursive(const std::filesystem::path& path,
                                  const bool& ignore_directories,
                                  const std::vector<std::regex>& ignored_files_directories);

} // namespace helpers

} // namespace fima
