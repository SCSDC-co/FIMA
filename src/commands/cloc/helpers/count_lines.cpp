/*
 * src/commands/cloc/helpers/count_lines.cpp
 * include/commands/cloc/helpers/count_lines.h
 *
 * Implementation of the line counter of `cloc`
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/cloc/helpers/count_lines.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "commands/cloc/helpers/Stats.h"
#include "mappings.h"
#include "utility/trim_string.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

namespace helpers {

fima::cloc::classes::Stats
count_lines(const fs::path& file_path, const fima::mappings::Comments& comments)
{
    fima::cloc::classes::Stats file_stats;

    std::ifstream file(file_path);

    if (file.is_open()) {
        std::string line;

        int code_lines{ 0 };
        int blank_lines{ 0 };
        int comment_lines{ 0 };

        bool can_have_single_comments{ comments.can_have_single_comment() };
        bool can_have_multiline_comments{ comments.can_have_multiline_comment() };
        bool is_multiline{ false };

        while (std::getline(file, line)) {
            fima::utility::trim(line);

            if (line.empty()) {
                ++blank_lines;

                continue;
            }

            if (can_have_single_comments &&
                ((!comments.single.empty() && line.starts_with(comments.single)) ||
                 (!comments.single_alt.empty() && line.starts_with(comments.single_alt)))) {
                ++comment_lines;

                continue;
            }

            if (can_have_multiline_comments) {
                if ((!comments.multiline_start.empty() && !comments.multiline_end.empty() &&
                     line.starts_with(comments.multiline_start) &&
                     line.ends_with(comments.multiline_end)) ||
                    (!comments.multiline_start_alt.empty() && !comments.multiline_end_alt.empty() &&
                     line.starts_with(comments.multiline_start_alt) &&
                     line.ends_with(comments.multiline_end_alt))) {
                    ++comment_lines;

                    continue;
                }

                if (is_multiline &&
                    !((!comments.multiline_end.empty() && line.ends_with(comments.multiline_end)) ||
                      (!comments.multiline_end_alt.empty() &&
                       line.ends_with(comments.multiline_end_alt)))) {
                    ++comment_lines;

                    continue;
                }

                if (!is_multiline && ((!comments.multiline_start.empty() &&
                                       line.starts_with(comments.multiline_start)) ||
                                      (!comments.multiline_start_alt.empty() &&
                                       line.starts_with(comments.multiline_start_alt)))) {
                    ++comment_lines;

                    is_multiline = true;

                    continue;
                }

                if (is_multiline && ((!comments.multiline_end.empty() &&
                                      line.starts_with(comments.multiline_end)) ||
                                     (!comments.multiline_end_alt.empty() &&
                                      line.starts_with(comments.multiline_end_alt)))) {
                    is_multiline = false;

                    ++comment_lines;

                    continue;
                }
            }

            ++code_lines;
        }

        file_stats.set_code(code_lines);
        file_stats.set_blank_lines(blank_lines);
        file_stats.set_comments(comment_lines);
    }

    file_stats.set_total();

    return file_stats;
}

} // namespace helpers

} // namespace cloc

} // namespace fima
