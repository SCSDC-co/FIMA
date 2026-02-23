/*
 * src/commands/rename.cpp
 * include/commands/rename.h
 *
 * The implementation of the `rename` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/rename.h"

#include <filesystem>
#include <string>

#include "logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

void
rename(const fs::path old_name, const fs::path new_name)
{
    if (!fs::exists(old_name)) {
        fima::logger::error(true,
                            "rename",
                            fima::colors::GREEN + "The item you want to rename does not exist: " +
                              fima::colors::RESET + "{}",
                            old_name.string());

        return;
    }

    if (fs::exists(new_name)) {
        fima::logger::error(true,
                            "rename",
                            std::string(fima::colors::RED) +
                              "The item already exists: " + std::string(fima::colors::RESET) + "{}",
                            new_name.string());

        return;
    }

    try {
        fs::rename(old_name, new_name);

        fima::logger::info(true,
                           "rename",
                           "{}" + fima::colors::GREEN + " renamed to: " + fima::colors::RESET +
                             "{}",
                           old_name.string(),
                           new_name.string());
    } catch (const std::exception& ex) {
        fima::logger::error(
          true, "rename", fima::colors::RED + "Failed to rename the item:" + fima::colors::RESET);

        fima::logger::error(true,
                            "rename",
                            fima::colors::RED + "  Old name: " + fima::colors::RESET + "{}",
                            old_name.string());
        fima::logger::error(true,
                            "rename",
                            fima::colors::RED + "  New name: " + fima::colors::RESET + "{}",
                            new_name.string());

        fima::logger::error(true, "rename", ex.what());
    }

    return;
}

} // namespace fima
