/*
 * include/utility/most_common.h
 *
 * A little utility to find the most common element in an iterable
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <algorithm>
#include <iterator>
#include <map>

namespace fima {

namespace utility {

template<class InputIt, class T = typename std::iterator_traits<InputIt>::value_type>
T
most_common(InputIt begin, InputIt end)
{
    std::map<T, int> counts{};

    for (InputIt it = begin; it != end; ++it) {
        if (counts.find(*it) != counts.end()) {
            ++counts[*it];
        } else {
            counts[*it] = 1;
        }
    }
    return std::max_element(counts.begin(),
                            counts.end(),
                            [](const std::pair<T, int>& pair1, const std::pair<T, int>& pair2) {
                                return pair1.second < pair2.second;
                            })
      ->first;
}

} // namespace utility

} // namespace fima
