/*
 * src/commands/create/file.cpp
 * include/commands/create/file.h
 *
 * The implementation of the `create ... file` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/create/file.h"

#include <exception>
#include <filesystem>
#include <vector>

#include "helpers/logger.h"
#include "utility/file.h"

namespace fs = std::filesystem;

namespace fima {

namespace create {

void
file(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {
        if (fs::is_regular_file(entry)) {
            fima::helpers::log(
              fima::helpers::logger_type::ERROR, "This file already exists: ", entry.string());

            continue;
        }

        try {
            fima::file::create(entry, "");

            fima::helpers::log(
              fima::helpers::logger_type::LOG, "File created at: ", entry.string());
        } catch (const std::exception& ex) {
            fima::helpers::log(
              fima::helpers::logger_type::ERROR, "Failed to create the file: ", entry.string());
            fima::helpers::log(fima::helpers::logger_type::ERROR, "", ex.what());
        }
    }
}

} // namespace create

} // namespace fima
