/*
 * include/helpers/get_data_path.h
 *
 * This is an helper for getting the data path for applications
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>

namespace fima {

namespace helpers {

inline std::filesystem::path
get_application_data_path()
{
#if defined(_WIN32) || defined(_WIN64)
    return std::filesystem::path(std::getenv("APPDATA"));
#elif defined(__APPLE__) || defined(__MACH__)
    return std::filesystem::path(std::string(std::getenv("HOME")) + "/Library/Application Support");
#elif defined(__linux__) || defined(__unix) || defined(__unix__) || defined(__FreeBSD__)
    return std::filesystem::path(std::string(std::getenv("HOME")) + "/.local/share");
#endif
}

} // namespace helpers

} // namespace fima
