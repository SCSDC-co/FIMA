/*
 * src/commands/copy/directory.cpp
 * include/commands/copy/directory.h
 *
 * The implementation of the `copy` subcommand when copying directories
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/copy/directory.h"

#include <filesystem>

#include "logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace commands {

namespace copy {

void
directory(fs::path source, fs::path destination)
{
    if (!fs::is_directory(source)) {
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "The source file is a file or does not exists: " +
                              fima::colors::RESET + "{}",
                            source.string());
    }

    if (!fs::is_directory(destination)) {
        fs::create_directory(destination);
    }

    try {
        fs::copy(
          source, destination, fs::copy_options::overwrite_existing | fs::copy_options::recursive);

        fima::logger::info(true,
                           "copy dir",
                           fima::colors::GREEN + "Directory " + fima::colors::RESET + "{}" +
                             fima::colors::GREEN + " copied to " + fima::colors::RESET + "{}",
                           source.string(),
                           destination.string());
    } catch (const std::exception& ex) {
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "Failed to copy the directory" +
                              fima::colors::RESET);
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "  Source directory: " + fima::colors::RESET + "{}",
                            source.string());
        fima::logger::error(true,
                            "copy dir",
                            fima::colors::RED + "  Destination: " + fima::colors::RESET + "{}",
                            destination.string());

        fima::logger::error(true, "copy dir", ex.what());
    }
}

} // namespace copy

} // namespace commands

} // namespace fima
