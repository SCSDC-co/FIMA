/*
 * src/commands/create/directory.cpp
 * include/commands/create/directory.h
 *
 * The implementation of the `create ... dir` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/create/directory.h"

#include <exception>
#include <filesystem>
#include <vector>

#include "helpers/logger.h"

namespace fs = std::filesystem;

namespace fima {

namespace create {

void
dir(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {

        if (fs::is_directory(entry)) {
            fima::helpers::log(
              fima::helpers::logger_type::ERROR, "This directory already exists: ", entry.string());

            continue;
        }

        try {
            fs::create_directories(entry);

            fima::helpers::log(
              fima::helpers::logger_type::LOG, "Directory created at: ", entry.string());
        } catch (const std::exception& ex) {
            fima::helpers::log(fima::helpers::logger_type::ERROR,
                               "Failed to create the directory: ",
                               entry.string());
            fima::helpers::log(fima::helpers::logger_type::ERROR, "", ex.what());
        }
    }
}

} // namespace create

} // namespace fima
