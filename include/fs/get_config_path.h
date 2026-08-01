/*
 * include/fs/get_config_path.h
 *
 * This is an helper for getting the config path for applications
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "theme.h"

namespace fima {

namespace fs {

inline std::filesystem::path
get_application_config_path()
{
#if defined(_WIN32) || defined(_WIN64)
    if (auto home_var = std::getenv("APPDATA")) {
        return std::filesystem::path(home_var);
    } else {
        std::cerr << fima::theme::theme.error << "FATAL ERROR: " << fima::theme::theme.secondary
                  << "Environment variable \"APPDATA\" is not set. FIMA can't run without it"
                  << fima::theme::Color::reset << '\n';

        std::exit(1);
    }
#elif defined(__linux__) || defined(__unix) || defined(__unix__) || defined(__FreeBSD__) ||        \
  defined(__APPLE__) || defined(__MACH__)
    if (auto home_var = std::getenv("HOME")) {
        return std::filesystem::path(std::string(home_var) + "/.config");
    } else {
        std::cerr << fima::theme::theme.error << "FATAL ERROR: " << fima::theme::theme.secondary
                  << "Environment variable \"HOME\" is not set. FIMA can't run without it"
                  << fima::theme::Color::reset << '\n';

        std::exit(1);
    }
#else
#  error "Platform not supported! Supported platforms: Windows, MacOS, Linux, any Unix-like OS"
#endif
}

} // namespace fs

} // namespace fima
