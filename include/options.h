/*
 * include/options.h
 *
 * A little header that contains all the options for the commands of FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <vector>

namespace fima {

namespace options {

struct cloc_options
{
    std::vector<std::filesystem::path> paths{ std::filesystem::current_path() };
    std::string sorting{ "total" };
    bool quiet{ false };
    bool show_languages{ false };
    bool gitignore{ false };
};

struct ls_options
{
    bool icons{ false };
    bool all{ false };
    bool long_output{ false };
    bool verbose{ false };
    bool gitignore{ false };
};

struct info_options
{
    std::filesystem::directory_entry path{ std::filesystem::current_path() };
    bool git{ false };
};

} // namespace options

} // namespace fima
