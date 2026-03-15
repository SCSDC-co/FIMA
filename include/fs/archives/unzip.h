/*
 * scr/fs/archives/unzip.cpp
 * include/fs/archives/unzip.h
 *
 * A utility to extract archives. It is a separate module because it needs to support many archive
 * formats.
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace fs {

namespace archives {

void
unzip(const std::filesystem::path& archive_to_unzip, const std::filesystem::path& output);

} // namespace archives

} // namespace fs

} // namespace fima
