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

#include "logger/logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace copy {

void
directory(fs::path source, fs::path destination)
{
    if (!fs::is_directory(source)) {
        fima::logger::log(fima::logger::Type::ERROR,
                          true,
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

        fima::logger::log(fima::logger::Type::INFO,
                          true,
                          fima::colors::GREEN + "Directory " + fima::colors::RESET + "{}" +
                            fima::colors::GREEN + " copied to " + fima::colors::RESET + "{}",
                          source.string(),
                          destination.string());
    } catch (const std::exception& ex) {
        fima::logger::log(fima::logger::Type::ERROR,
                          true,
                          fima::colors::RED + "Failed to copy the directory" + fima::colors::RESET);
        fima::logger::log(fima::logger::Type::ERROR,
                          true,
                          fima::colors::RED + "  Source directory: " + fima::colors::RESET + "{}",
                          source.string());
        fima::logger::log(fima::logger::Type::ERROR,
                          true,
                          fima::colors::RED + "  Destination: " + fima::colors::RESET + "{}",
                          destination.string());

        fima::logger::log(fima::logger::Type::ERROR, true, ex.what());
    }
}

} // namespace copy

} // namespace fima
