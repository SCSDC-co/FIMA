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
#include <iostream>

#include "helpers/logger.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace copy {

void
file(const fs::path source, const fs::path destination)
{
    if (!fs::is_regular_file(source)) {
        fima::helpers::log(fima::helpers::logger_type::ERROR,
                           "The source file is a directory or does not exists: ",
                           source.string());
    }

    try {
        fs::copy_file(source, destination, fs::copy_options::overwrite_existing);

        std::clog << fima::colors::GREEN << "File " << fima::colors::RESET << source.string()
                  << fima::colors::GREEN << " copied to " << fima::colors::RESET
                  << destination.string() << '\n';
    } catch (const std::exception& ex) {
        fima::helpers::log(fima::helpers::logger_type::ERROR, "Failed to copy the file: ", "");

        fima::helpers::log(fima::helpers::logger_type::ERROR, "  Source file: ", source);
        fima::helpers::log(fima::helpers::logger_type::ERROR, "  Destination: ", destination);

        fima::helpers::log(fima::helpers::logger_type::ERROR, "", ex.what());
    }
}

} // namespace copy

} // namespace fima
