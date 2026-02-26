/*
 * src/utility/file.cpp
 * include/utility/file.h
 *
 * A module for doing general file operations
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace fs {

namespace operations {

size_t
get_file_size(const std::filesystem::path& path);

std::filesystem::file_time_type
get_file_time(const std::filesystem::path& path);

std::string
get_file_owner(const std::filesystem::path& path);

bool
is_file_executable(const std::filesystem::path& path);

bool
is_compressed_archive(const std::filesystem::path& path);

bool
is_media(const std::filesystem::path& path);

void
create(const std::filesystem::path& path, const std::string_view& conent);

void
overwrite(const std::filesystem::path& path, const std::string_view& content);

} // namespace operations

} // namespace fs

} // namespace fima
