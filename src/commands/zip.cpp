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

#include "commands/zip.h"

#include <filesystem>
#include <iostream>
#include <vector>

#include "fs/archive.h"
#include "logger.h"
#include "utility/colors.h"

namespace fima {

namespace commands {

void
zip(const std::vector<std::filesystem::path>& items_to_zip, const std::filesystem::path& output)
{
    if (std::filesystem::exists(output)) {
        fima::logger::error(true,
                            "zip",
                            fima::colors::RED +
                              "The archive already exists: " + fima::colors::RESET + "{}",
                            output.string());

        return;
    }

    fima::fs::archive::zip(items_to_zip, output);

    std::cout << fima::colors::GREEN + "Zipped files to: " + fima::colors::RESET << output.string()
              << '\n';
}

} // namespace commands

} // namespace fima
