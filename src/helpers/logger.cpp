/*
 * src/helpers/logger.cpp
 * include/helpers/logger.h
 *
 * This is the implementation of the logger of FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "helpers/logger.h"

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

#include "utility/colors.h"

inline std::filesystem::path
get_log_file()
{
    namespace fs = std::filesystem;

    fs::path temp = fs::temp_directory_path();

    fs::path newest;
    fs::file_time_type newest_time;

    bool found = false;

    for (const auto& entry : fs::directory_iterator(temp)) {
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

        fs::file_time_type time = entry.last_write_time();

        if (!found || time > newest_time) {
            newest      = entry.path();
            newest_time = time;
            found       = true;
        }
    }

    if (found) {
        return newest;
    }

    // If no file is found it will create a new one

    std::chrono::time_point now  = std::chrono::system_clock::now();
    std::chrono::time_point time = std::chrono::floor<std::chrono::seconds>(now);

    std::string current_time = std::format("{:%Y%b%d_%H%M%S}", time);

    return temp / ("fima_" + current_time + ".log");
}

namespace fima {

namespace helpers {

void
log(logger_type type, const std::string& message, const std::string& additional_message)
{
    std::filesystem::path path = get_log_file();

    static std::ofstream log_file;
    log_file.open(path, std::ios::app);

    std::chrono::time_point now  = std::chrono::system_clock::now();
    std::chrono::time_point time = std::chrono::floor<std::chrono::seconds>(now);

    std::string current_time = std::format("{:%Y%b%d_%H%M%S}", time);

    switch (type) {
        case logger_type::LOG:
            std::clog << fima::colors::GREEN << message << fima::colors::RESET << additional_message
                      << '\n';

            if (log_file.is_open()) {
                log_file << "[" << current_time << "]  " << "[LOG] " << message
                         << additional_message << '\n';
            }

            break;

        case logger_type::ERROR:
            std::cerr << fima::colors::RED << message << fima::colors::RESET << additional_message
                      << std::endl;

            if (log_file.is_open()) {
                log_file << "[" << current_time << "]  " << "[ERROR] " << message
                         << additional_message << '\n';
            }

            break;
    }

    log_file.close();
}

}

}
