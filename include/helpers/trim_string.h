/*
 * include/helpers/trim_string.h
 *
 * This is a little helper for trimming strings
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <algorithm>
#include <string>

inline void
ltrim(std::string& s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

inline void
rtrim(std::string& s)
{
    s.erase(
      std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(),
      s.end());
}
namespace fima {

namespace helpers {

inline std::string
trim(std::string& s)
{
    rtrim(s);
    ltrim(s);

    return s;
}

}

}
