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
    std::vector<std::filesystem::path> paths_to_ignore{};
    std::string sorting{ "total" };
    bool quiet{ false };
    bool show_languages{ false };
    bool gitignore{ true };
};

struct ls_options
{
    std::vector<std::filesystem::path> paths{ std::filesystem::current_path() };
    bool icons{ false };
    bool all{ false };
    bool long_output{ false };
    bool verbose{ false };
    bool headers{ false };
    bool one_line{ false };
    bool directory_first{ false };
    bool directory_last{ false };
};

struct info_options
{
    std::filesystem::directory_entry file_path{};
    bool git{ false };
    bool tags{ false };
    bool remote{ false };
    bool verbose{ false };
};

struct tree_options
{
    std::string prefix{ "" };
    bool tui{ false };
    bool all{ false };
    bool gitignore{ true };
    bool verbose{ false };
};

struct rm_options
{
    std::vector<std::string> paths{};
    bool recursive{ false };
    bool verbose{ false };
    bool no_trash{ false };
};

} // namespace options

} // namespace fima
