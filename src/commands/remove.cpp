/*
 * src/commands/remove.cpp
 * include/commands/remove.h
 *
 * The implementation of the `remove` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/remove.h"

#include <filesystem>
#include <vector>

#include "logger/logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

void
remove(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {
        if (!fs::exists(entry)) {
            fima::logger::log(fima::logger::Type::ERROR,
                              true,
                              fima::colors::RED +
                                "The path doesn't exists: " + fima::colors::RESET + "{}",
                              entry.string());

            continue;
        }

        try {
            fs::remove_all(entry);

            fima::logger::log(fima::logger::Type::INFO,
                              true,
                              fima::colors::GREEN + "Item removed: " + fima::colors::RESET + "{}",
                              entry.string());
        } catch (const std::exception& ex) {
            fima::logger::log(fima::logger::Type::ERROR,
                              true,
                              fima::colors::RED +
                                "Failed to remove directory: " + fima::colors::RESET + "{}",
                              entry.string());
            fima::logger::log(fima::logger::Type::ERROR, true, ex.what());
        }
    }
}

} // namespace fima
