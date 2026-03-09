/*
 * src/commands/unzip.cpp
 * include/commands/unzip.h
 *
 * A command to unzip .zip archives
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace commands {

void
unzip(const std::filesystem::path& arhcive_to_unzip, const std::filesystem::path& output);

}

} // namespace fima
