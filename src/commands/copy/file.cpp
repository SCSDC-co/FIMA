/*
 * src/commands/copy/file.cpp
 * include/commands/copy/file.h
 *
 * The implementation of the `copy` subcommand when copying files
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/copy/file.h"

#include <filesystem>

#include "logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace copy {

void
file(const fs::path source, const fs::path destination)
{
    if (!fs::is_regular_file(source)) {
        fima::logger::error(
          true,
          "copy file",
          fima::colors::RED +
            "The source file is a directory or does not exists: " + fima::colors::RESET + "{}",
          source.string());
    }

    try {
        fs::copy_file(source, destination, fs::copy_options::overwrite_existing);

        fima::logger::info(true,
                           "copy file",
                           fima::colors::GREEN + "File " + fima::colors::RESET + "{}" +
                             fima::colors::GREEN + " copied to " + fima::colors::RESET + "{}",
                           source.string(),
                           destination.string());
    } catch (const std::exception& ex) {
        fima::logger::error(
          true, "copy file", fima::colors::RED + "Failed to copy the file" + fima::colors::RESET);
        fima::logger::error(true,
                            "copy file",
                            fima::colors::RED + "  Source directory: " + fima::colors::RESET + "{}",
                            source.string());
        fima::logger::error(true,
                            "copy file",
                            fima::colors::RED + "  Destination: " + fima::colors::RESET + "{}",
                            destination.string());

        fima::logger::error(true, "copy file", ex.what());
    }
}

} // namespace copy

} // namespace fima
