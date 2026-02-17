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
#include <iostream>

#include "utility/colors.h"
#include "helpers/logger.h"

namespace fs = std::filesystem;

namespace fima {

void
rename(const fs::path old_name, const fs::path new_name)
{
    if (!fs::exists(old_name)) {
        fima::helpers::log(fima::helpers::logger_type::ERROR,
                           "The item you want to rename does not exist: ",
                           old_name.string());

        return;
    }

    if (fs::exists(new_name)) {
        fima::helpers::log(
          fima::helpers::logger_type::ERROR, "The item already exists: ", new_name.string());

        return;
    }

    try {
        fs::rename(old_name, new_name);

        std::clog << old_name << fima::colors::GREEN << " renamed to " << fima::colors::RESET
                  << new_name << '\n';
    } catch (const std::exception& ex) {
        fima::helpers::log(fima::helpers::logger_type::ERROR, "Failed to rename the item: ", "");

        fima::helpers::log(fima::helpers::logger_type::ERROR, "  Old name: ", old_name.string());
        fima::helpers::log(fima::helpers::logger_type::ERROR, "  New name: ", new_name.string());

        fima::helpers::log(fima::helpers::logger_type::ERROR, "", ex.what());
    }

    return;
}

} // namespace fima
