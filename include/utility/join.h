/*
 * include/utility/join.h
 *
 * A little utility to join vectors
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <string>
#include <vector>

namespace fima {

namespace utility {

inline std::string
join(const std::vector<std::string>& vector, const std::string& sep)
{
    std::string out{};

    for (size_t i = 0; i < vector.size(); ++i) {
        out += vector[i];

        if (i + 1 < vector.size()) {
            out += sep;
        }
    }

    return out;
}

} // namespace utility

} // namespace fima
