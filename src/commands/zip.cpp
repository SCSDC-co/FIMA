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

#include "fs/archives/zip.h"
#include "logger.h"
#include "utility/colors.h"

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

    fima::fs::archives::zip::create_archive(items_to_zip, output);

    std::cout << fima::colors::GREEN + "Zipped files to: " + fima::colors::RESET << output.string()
              << '\n';
}

namespace fima {

namespace commands {

void
setup_zip(CLI::App& app,
          std::vector<std::filesystem::path>& paths,
          std::filesystem::path& output_path)
{
    CLI::App* subcmd =
      app.add_subcommand("zip", "Zip files into a .zip archive")->configurable(false);

    subcmd->add_option("paths", paths, "File to zip")->required(true)->configurable(false);
    subcmd->add_option("-o,--output", output_path, "The output archive")
      ->required(true)
      ->configurable(false);

    subcmd->usage("fima zip [PATHS] -o [OUTPUT]");

    subcmd->callback([&]() { zip(paths, output_path); });
}

} // namespace commands

} // namespace fima
