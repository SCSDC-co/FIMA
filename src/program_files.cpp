/*
 * src/program_files.cpp
 * include/program_files.h
 *
 * A utility to setup the program files (like the config file, and the JSON files)
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "program_files.h"

#include <filesystem>

#include "logger/logger.h"
#include "utility/colors.h"
#include "utility/file.h"

namespace fs = std::filesystem;

namespace fima {

namespace program_files {

void
create_config_dirs()
{
    if (!fs::exists(FIMA_CONFIG_PATH)) {
        fs::create_directory(FIMA_CONFIG_PATH);
    }

    if (!fs::exists(MAPPINGS_PATH)) {
        fs::create_directory(MAPPINGS_PATH);
    }
}

void
create_config_files()
{
    create_config_dirs();

    if (!fs::exists(CONFIG_FILE_PATH)) {
        fima::file::create(CONFIG_FILE_PATH, "");
    }

    if (!fs::exists(LANGUAGES_FILE_PATH)) {
        fima::file::create(LANGUAGES_FILE_PATH, languages_file);
    }

    if (!fs::exists(MAP_LANGUAGES_ICON_PATH)) {
        fima::file::create(MAP_LANGUAGES_ICON_PATH, map_languages_icon);
    }

    if (!fs::exists(MAP_DIRECTORY_ICON_PATH)) {
        fima::file::create(MAP_DIRECTORY_ICON_PATH, map_directory_icon);
    }

    if (!fs::exists(MAP_LANGUAGES_FAMILY_PATH)) {
        fima::file::create(MAP_LANGUAGES_FAMILY_PATH, map_languages_family);
    }

    if (!fs::exists(MAP_LANGUAGES_NAME_PATH)) {
        fima::file::create(MAP_LANGUAGES_NAME_PATH, map_languages_name);
    }
}

void
reset_config_files(const bool& preserve_config_file)
{
    create_config_dirs();

    if (!preserve_config_file) {
        fima::file::overwrite(CONFIG_FILE_PATH, "");
    }

    fima::file::overwrite(LANGUAGES_FILE_PATH, languages_file);
    fima::file::overwrite(MAP_LANGUAGES_ICON_PATH, map_languages_icon);
    fima::file::overwrite(MAP_DIRECTORY_ICON_PATH, map_directory_icon);
    fima::file::overwrite(MAP_LANGUAGES_FAMILY_PATH, map_languages_family);
    fima::file::overwrite(MAP_LANGUAGES_NAME_PATH, map_languages_name);

    fima::logger::log(fima::logger::Type::INFO,
                      true,
                      "reset config files",
                      fima::colors::GREEN +
                        "Successfully reset all the config file, config file preserved: " +
                        fima::colors::RESET + (preserve_config_file ? "yes" : "no"));
}

} // namespace program_files

} // namespace fima
