/*
 * include/config.h
 * src/config.cpp
 *
 * This file contains the variables that should be available in all the program
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "config.h"

#include <toml++/toml.hpp>

#include "fs/get_config_path.h"
#include "fs/operations.h"

namespace fima {

namespace config {

void
setup_variables()
{
    CONFIG_PATH      = fima::fs::get_application_config_path();
    FIMA_CONFIG_PATH = CONFIG_PATH / "fima";
    CONFIG_FILE_PATH = FIMA_CONFIG_PATH / "config.toml";
}

void
create_config_file()
{
    if (!std::filesystem::exists(FIMA_CONFIG_PATH)) {
        std::filesystem::create_directory(FIMA_CONFIG_PATH);
    }

    if (!std::filesystem::exists(CONFIG_FILE_PATH)) {
        fima::fs::operations::create(CONFIG_FILE_PATH, "");
    }
}

void
parse_config_file()
{
    auto config = toml::parse_file(CONFIG_FILE_PATH.string());

    depth                  = config["depth"].value_or(8);
    process_directory_size = config["process_directory_size"].value_or(false);
}

} // namespace config

} // namespace fima
