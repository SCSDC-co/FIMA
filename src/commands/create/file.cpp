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

#include "fs/operations.h"
#include "logger.h"
#include "utility/colors.h"

namespace fima {

namespace commands {

namespace create {

void
file(const std::vector<std::filesystem::path>& paths)
{
    for (const auto& entry : paths) {
        if (std::filesystem::exists(entry)) {
            fima::logger::error(true,
                                "create file",
                                fima::colors::RED +
                                  "This item already exists: " + fima::colors::RESET + "{}",
                                entry.string());

            continue;
        }

        try {
            fima::fs::operations::create(entry, "");

            fima::logger::info(true,
                               "create file",
                               fima::colors::GREEN + "File created at: " + fima::colors::RESET +
                                 "{}",
                               entry.string());
        } catch (const std::exception& ex) {
            fima::logger::error(true,
                                "create file",
                                fima::colors::RED +
                                  "Failed to create the file: " + fima::colors::RESET + "{}",
                                entry.string());
            fima::logger::error(true, "create file", ex.what());
        }
    }
}

} // namespace create

} // namespace commands

} // namespace fima
