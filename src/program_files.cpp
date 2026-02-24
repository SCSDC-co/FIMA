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

#include "fs/filesystem_op.h"
#include "logger.h"
#include "utility/colors.h"

namespace fima {

namespace program_files {

void
create_config_dirs()
{
    if (!std::filesystem::exists(FIMA_CONFIG_PATH)) {
        std::filesystem::create_directory(FIMA_CONFIG_PATH);
    }

    if (!std::filesystem::exists(MAPPINGS_PATH)) {
        std::filesystem::create_directory(MAPPINGS_PATH);
    }
}

void
create_config_files()
{
    create_config_dirs();

    if (!std::filesystem::exists(CONFIG_FILE_PATH)) {
        fima::fs::operations::create(CONFIG_FILE_PATH, "");
    }

    if (!std::filesystem::exists(LANGUAGES_FILE_PATH)) {
        fima::fs::operations::create(LANGUAGES_FILE_PATH, languages_file);
    }

    if (!std::filesystem::exists(MAP_LANGUAGES_ICON_PATH)) {
        fima::fs::operations::create(MAP_LANGUAGES_ICON_PATH, map_languages_icon);
    }

    if (!std::filesystem::exists(MAP_DIRECTORY_ICON_PATH)) {
        fima::fs::operations::create(MAP_DIRECTORY_ICON_PATH, map_directory_icon);
    }

    if (!std::filesystem::exists(MAP_LANGUAGES_FAMILY_PATH)) {
        fima::fs::operations::create(MAP_LANGUAGES_FAMILY_PATH, map_languages_family);
    }

    if (!std::filesystem::exists(MAP_LANGUAGES_NAME_PATH)) {
        fima::fs::operations::create(MAP_LANGUAGES_NAME_PATH, map_languages_name);
    }
}

void
reset_config_files(const bool& preserve_config_file)
{
    create_config_dirs();

    if (!preserve_config_file) {
        fima::fs::operations::overwrite(CONFIG_FILE_PATH, "");
    }

    fima::fs::operations::overwrite(LANGUAGES_FILE_PATH, languages_file);
    fima::fs::operations::overwrite(MAP_LANGUAGES_ICON_PATH, map_languages_icon);
    fima::fs::operations::overwrite(MAP_DIRECTORY_ICON_PATH, map_directory_icon);
    fima::fs::operations::overwrite(MAP_LANGUAGES_FAMILY_PATH, map_languages_family);
    fima::fs::operations::overwrite(MAP_LANGUAGES_NAME_PATH, map_languages_name);

    logger::info(true,
                 "reset config files",
                 fima::colors::GREEN +
                   "Successfully reset all the config file, config file preserved: " +
                   fima::colors::RESET + (preserve_config_file ? "true" : "false"));
}

} // namespace program_files

} // namespace fima
