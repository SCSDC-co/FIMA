/*
 * scr/fs/archive.cpp
 * include/fs/archive.h
 *
 * A module that contains some useful functions for working with archives
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <vector>

namespace fima {

namespace fs {

namespace archive {

void
zip(const std::vector<std::filesystem::path>& files_to_zip, const std::filesystem::path& output);

void
unzip(const std::filesystem::path& file_to_unzip, const std::filesystem::path& output);

} // namespace archive

} // namespace fs

} // namespace fima
