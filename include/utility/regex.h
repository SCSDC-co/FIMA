/*
 * include/utility/regex.h
 *
 * A little utility to work with regex
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <regex>
#include <vector>

namespace fima {

namespace helpers {

namespace regex {

inline bool
matches_any_regex(const std::string& s, const std::vector<std::regex>& regexes)
{
    for (const auto& rgx : regexes) {
        if (std::regex_match(s, rgx)) {
            return true;
        }
    }
    return false;
}

inline std::regex
glob_to_regex(const std::string& glob)
{
    std::string regex_str;
    for (char c : glob) {
        switch (c) {
            case '*':
                regex_str += ".*";
                break;
            case '?':
                regex_str += '.';
                break;
            case '.':
                regex_str += "\\.";
                break;
            default:
                regex_str += c;
                break;
        }
    }
    return std::regex(regex_str);
}

} // namespace regex

} // namespace helpers

} // namespace fima
