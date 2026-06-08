/*
 * scr/fs/archives/zip.cpp
 * include/fs/archives/zip.h
 *
 * A utility to work with .zip archives
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

namespace archives {

namespace zip {

void
create_archive(const std::vector<std::filesystem::path>& files_to_zip,
               const std::filesystem::path& output);

}

} // namespace archives

} // namespace fs

} // namespace fima
