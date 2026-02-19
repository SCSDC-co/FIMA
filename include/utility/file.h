/*
 * src/utility/file.cpp
 * include/utility/file.h
 *
 * A utility for getting some file information
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace file {

size_t
get_file_size(const std::filesystem::path& path);

std::filesystem::file_time_type
get_file_time(const std::filesystem::path& path);

std::string
get_file_owner(const std::filesystem::path& path);

} // namespace file

} // namespace fima
