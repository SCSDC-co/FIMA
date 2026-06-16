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

#include "program_files.h"

namespace fima {

namespace config {

void
parse_config_file()
{
    auto config = toml::parse_file(fima::program_files::CONFIG_FILE_PATH.string());

    depth = config["depth"].value_or(8);
}

} // namespace config

} // namespace fima
