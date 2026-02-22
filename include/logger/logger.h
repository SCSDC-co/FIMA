/*
 * helper/logger/logger.h
 *
 * The main file of the logger of FIMA
 * This module is header-only because it uses templates
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "utility/get_current_time.h"

namespace fima {

namespace logger {

enum class Type
{
    INFO,
    ERROR
};

inline std::filesystem::path
get_log_file()
{
    std::filesystem::path temp_path = std::filesystem::temp_directory_path();

    auto time = fima::utility::get_current_time();

    std::filesystem::path newest;
    std::filesystem::file_time_type newest_time;

    bool found = false;

    for (const auto& entry : std::filesystem::directory_iterator(temp_path)) {
        if (found) {
            break;
        }

        if (!entry.is_regular_file()) {
            continue;
        }

        std::string name = entry.path().filename().string();

        if (!name.starts_with("fima_") || !name.ends_with(".log")) {
            continue;
        }

        std::filesystem::file_time_type time = entry.last_write_time();

        if (!found || time > newest_time) {
            newest      = entry.path();
            newest_time = time;
            found       = true;
        }
    }

    if (found) {
        return newest;
    }

    auto time_with_rounded_seconds = std::chrono::floor<std::chrono::seconds>(time.get_sys_time());

    std::string current_time = std::format("{:%Y%b%d_%H%M%S}", time_with_rounded_seconds);

    return temp_path / ("fima_" + current_time + ".log");
}

// This is the main function of the logger, make sure to pass the correct number of values for the
// number of placeholders
template<typename... Args>
void
log(const Type& type,
    const bool& console_output,
    const std::string& command,
    const std::string& format,
    const Args&&... values)
{
    std::regex color_escape("\033\[[0-9;]*m");

    std::string formatted_string           = std::vformat(format, std::make_format_args(values...));
    std::string formatted_string_sanitized = std::regex_replace(formatted_string, color_escape, "");

    auto current_time = fima::utility::get_current_time();

    auto time = std::format("{:%Y/%b/%d %H:%M:%S}", current_time);

    auto log_file_line = [&](std::string type) {
        return std::format("[{}] [{}] ({}) {}\n", time, type, command, formatted_string_sanitized);
    };

    std::filesystem::path log_file_path = get_log_file();

    std::ofstream logfile(log_file_path, std::ios::app);

    switch (type) {
        case Type::INFO:
            if (console_output) {
                std::clog << formatted_string << '\n';
            }

            if (logfile.is_open()) {
                logfile << log_file_line("INFO");
            }

            break;
        case Type::ERROR:
            if (console_output) {
                std::cerr << formatted_string << '\n';
            }

            if (logfile.is_open()) {
                logfile << log_file_line("ERROR");
            }

            break;
    }

    logfile.close();
}

} // namespace logger

} // namespace fima
