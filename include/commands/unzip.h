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

#include "cli/CLI11.hpp"

namespace fima {

namespace commands {

void
setup_unzip(CLI::App& app, std::filesystem::path& archive, std::filesystem::path& output_path);

}

} // namespace fima
