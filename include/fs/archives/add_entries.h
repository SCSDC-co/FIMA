/*
 * scr/fs/archives/add_entries.cpp
 * include/fs/archives/add_entries.h
 *
 * Adds entries to a .zip archive
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
add_entries(const std::vector<std::filesystem::path>& entries_to_add,
            const std::filesystem::path& archive_path);

}

} // namespace archives

} // namespace fs

} // namespace fima
