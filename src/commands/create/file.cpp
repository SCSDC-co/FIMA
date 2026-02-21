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

#include "logger/logger.h"
#include "utility/colors.h"
#include "utility/file.h"

namespace fs = std::filesystem;

namespace fima {

namespace create {

void
file(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {
        if (fs::is_regular_file(entry)) {
            fima::logger::log(fima::logger::Type::ERROR,
                              true,
                              fima::colors::RED +
                                "This file already exists: " + fima::colors::RESET + "{}",
                              entry.string());

            continue;
        }

        try {
            fima::file::create(entry, "");

            fima::logger::log(fima::logger::Type::INFO,
                              true,
                              fima::colors::GREEN + "File created at: " + fima::colors::RESET +
                                "{}",
                              entry.string());
        } catch (const std::exception& ex) {
            fima::logger::log(fima::logger::Type::ERROR,
                              true,
                              fima::colors::RED +
                                "Failed to create the file: " + fima::colors::RESET + "{}",
                              entry.string());
            fima::logger::log(fima::logger::Type::ERROR, true, ex.what());
        }
    }
}

} // namespace create

} // namespace fima
