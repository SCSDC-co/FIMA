/*
 * src/commands/zip.cpp
 * include/commands/zip.h
 *
 * A command to create .zip archives
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <vector>

namespace fima {

namespace commands {

void
zip(const std::vector<std::filesystem::path>& items_to_zip, const std::filesystem::path& output);

}

} // namespace fima
