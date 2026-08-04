/*
 * include/utility/regex.h
 *
 * A little utility to work with regex
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <regex>
#include <vector>

namespace fima {

namespace utility {

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

// this code is taken from https://github.com/SudoMaker/ReGlob/
// thank guys ❤️
inline std::string
glob_to_regex(const std::string& glob)
{
    enum domains
    {
        DOMAIN_NONE           = 0,
        DOMAIN_SQUARE_BRACKET = 1,
        DOMAIN_BRACES         = 2
    };

    std::string regexp_str;

    bool escaped = false;

    std::vector<uint8_t> domain(1, DOMAIN_NONE);

    char c;
    uint8_t current_domain;
    for (size_t i = 0, len = glob.size(); i < len; i++) {
        c = glob[i];

        current_domain = domain.back();

        switch (c) {
            case '\\':
                if (escaped) {
                    escaped = false;
                    regexp_str += "\\\\";
                } else {
                    escaped = true;
                }
                break;
            case '$':
            case '^':
            case '+':
            case '.':
            case '(':
            case ')':
            case '=':
            case '!':
            case '|':
                if (!escaped) {
                    regexp_str += '\\';
                }
                regexp_str += c;
                escaped = false;
                break;
            case '?':
                if (escaped) {
                    regexp_str += c;
                } else {
                    regexp_str += "(.)";
                }
                escaped = false;
                break;

            case '[':
                if (!escaped) {
                    if (std::find(domain.begin(), domain.end(), DOMAIN_SQUARE_BRACKET) !=
                        domain.end()) {
                        std::cerr << "ReGlob: Cannot group square bracket inside square brackets."
                                  << '\n';

                        std::exit(1);
                    }
                    regexp_str += '(';
                    domain.push_back(DOMAIN_SQUARE_BRACKET);
                }
                regexp_str += c;
                escaped = false;
                break;

            case ']':
                if (escaped) {
                    regexp_str += c;
                } else {
                    if (current_domain == DOMAIN_SQUARE_BRACKET) {
                        regexp_str += "])";
                        domain.pop_back();
                    } else {
                        regexp_str += "\\]";
                    }
                }
                escaped = false;
                break;

            case '{':
                if (!escaped) {
                    domain.push_back(DOMAIN_BRACES);
                    regexp_str += '(';
                    break;
                }
                regexp_str += c;
                escaped = false;
                break;

            case '}':
                if (!escaped) {
                    if (current_domain == DOMAIN_BRACES) {
                        domain.pop_back();
                        regexp_str += ")";
                        break;
                    } else {
                        regexp_str += '\\';
                    }
                }
                regexp_str += c;
                escaped = false;
                break;

            case ',':
                if (!escaped && current_domain != DOMAIN_NONE) {
                    if (current_domain == DOMAIN_BRACES) {
                        regexp_str += '|';
                    } else if (current_domain == DOMAIN_SQUARE_BRACKET) {
                        regexp_str += c;
                    }
                } else {
                    regexp_str += "\\,";
                }
                escaped = false;
                break;

            case '*': {
                if (escaped) {
                    regexp_str += c;
                } else {
                    while (glob[i + 1] == '*') {
                        i++;
                    }

                    if (!escaped) {
                        regexp_str += "(.*)";
                    } else {
                        regexp_str += ".*";
                    }
                }
                escaped = false;
                break;
            }
            default:
                regexp_str += c;
                escaped = false;
        }
    }

    current_domain = domain.back();

    if (escaped) {
        std::cerr << "ReGlob: Escape at the end of line." << '\n';

        std::exit(1);
    }
    if (current_domain != DOMAIN_NONE) {
        std::cerr << "ReGlob: unmatched " +
                       std::string(current_domain == DOMAIN_SQUARE_BRACKET ? "]" : "}") +
                       " in glob expression"
                  << '\n';

        std::exit(1);
    }

    regexp_str = "^" + regexp_str + "$";

    return regexp_str;
}

} // namespace regex

} // namespace utility

} // namespace fima
