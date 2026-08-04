/*
 * src/fs/trash.cpp
 * include/fs/trash.h
 *
 * Trash system for FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace fs {

namespace trash {

inline std::filesystem::path TRASH_PATH{};
inline std::filesystem::path TRASH_FILES_PATH{};
inline std::filesystem::path TRASH_TRASHINFO_PATH{};

void
setup_variables();

void
add_file_to_trash(const std::filesystem::path& path);

} // namespace trash

} // namespace fs

} // namespace fima
