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

#include <format>
#include <iostream>
#include <string>

namespace fima {

namespace logger {

enum class Type
{
    INFO,
    ERROR
};

// This is the main function of the logger, make sure to pass the correct number of values for the
// number of placeholders
template<typename... Args>
void
log(const Type& type, const bool& console_output, const std::string& format, const Args&&... values)
{
    const std::string formatted_string = std::vformat(format, std::make_format_args(values...));

    switch (type) {
        case Type::INFO:
            std::clog << formatted_string << '\n';

            break;
        case Type::ERROR:
            std::cerr << formatted_string << '\n';

            break;
    }
}

} // namespace logger

} // namespace fima
