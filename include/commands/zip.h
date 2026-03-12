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

#include "cli/CLI11.hpp"

namespace fima {

namespace commands {

void
setup_zip(CLI::App& app,
          std::vector<std::filesystem::path>& paths,
          std::filesystem::path& output_path);

}

} // namespace fima
