/*
 * src/commands/unzip.cpp
 * include/commands/unzip.h
 *
 * A command to unzip .zip archives
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */
#include "commands/unzip.h"

#include <filesystem>
#include <iostream>

#include "fs/archives/unzip.h"
#include "utility/colors.h"

void
unzip(const std::filesystem::path& arhcive_to_unzip, const std::filesystem::path& output)
{
    if (!std::filesystem::exists(arhcive_to_unzip)) {
        std::cerr << fima::colors::RED << "The archive doesn't exists: " << fima::colors::RESET
                  << arhcive_to_unzip.string() << '\n';

        return;
    }

    if (std::filesystem::exists(output)) {
        std::cerr << fima::colors::RED
                  << "The output directory already exists: " << fima::colors::RESET
                  << output.string() << '\n';

        return;
    }

    fima::fs::archives::unzip(arhcive_to_unzip, output);

    std::cout << fima::colors::GREEN << "Archive " << fima::colors::RESET
              << arhcive_to_unzip.string() << fima::colors::GREEN
              << " unzipped to: " << fima::colors::RESET << output.string() << '\n';
}

namespace fima {

namespace commands {

void
setup_unzip(CLI::App& app, std::filesystem::path& archive, std::filesystem::path& output_path)
{

    CLI::App* subcmd = app.add_subcommand("unzip", "Unzip .zip archives")->configurable(false);

    subcmd->add_option("archive", archive, "The archive to unzip")
      ->required(true)
      ->configurable(false);
    subcmd->add_option("-o,--output", output_path, "The output of the unzipped archive")
      ->required(true)
      ->configurable(false);

    subcmd->usage("fima unzip [ARCHIVE] -o [OUTPUT]");

    subcmd->callback([&]() { unzip(archive, output_path); });
}

} // namespace commands

} // namespace fima
