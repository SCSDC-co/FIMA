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

#include "logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace create {

void
dir(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {
        if (fs::exists(entry)) {
            fima::logger::error(true,
                                "create dir",
                                fima::colors::RED +
                                  "This item already exists: " + fima::colors::RESET + "{}",
                                entry.string());

            continue;
        }

        try {
            fs::create_directories(entry);

            fima::logger::info(true,
                               "create dir",
                               fima::colors::GREEN +
                                 "Directory created at: " + fima::colors::RESET + "{}",
                               entry.string());
        } catch (const std::exception& ex) {
            fima::logger::error(true,
                                "create dir",
                                fima::colors::RED +
                                  "Failed to create the directory: " + fima::colors::RESET + "{}",
                                entry.string());
            fima::logger::error(true, "create dir", ex.what());
        }
    }
}

} // namespace create

} // namespace fima
