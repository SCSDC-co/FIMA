/*
 * helper/logger/logger.h
 *
 * A utility to get the current time
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <chrono>

namespace fima {

namespace utility {

inline auto
get_current_time()
{
    auto now                   = std::chrono::system_clock::now();
    auto now_with_rounded_secs = std::chrono::floor<std::chrono::seconds>(now);

    auto sys = std::chrono::clock_cast<std::chrono::system_clock>(now_with_rounded_secs);

    auto local = std::chrono::zoned_time{ std::chrono::current_zone(), sys };

    return local;
}

} // namespace utility

} // namespace fima
