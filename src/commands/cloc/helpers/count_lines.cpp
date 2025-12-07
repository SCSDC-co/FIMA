#include "../../../../include/commands/cloc/helpers/count_lines.h"

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

#include "../../../../include/commands/cloc/helpers/is_comment.h"
#include "../../../../include/commands/cloc/helpers/regex.h"
#include "../../../../include/commands/cloc/helpers/types.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

fima::cloc::FileStats cout_lines(fs::path path) {
    int number_of_lines_total{0};
    int number_of_lines_blank{0};
    int number_of_lines_code{0};
    int number_of_lines_comment{0};

    bool is_multi_line{false};

    std::string line;
    std::ifstream file(path);

    fima::cloc::FileStats file_stats;

    while (std::getline(file, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            ++number_of_lines_blank;

            continue;
        }

        if (is_comment(line)) {
            ++number_of_lines_comment;
        }

        if (!is_multi_line &&
            std::regex_match(line,
                             std::regex(regex::MULTI_LINE_COMMENT_START))) {

            is_multi_line = true;
            ++number_of_lines_comment;

            continue;
        }

        if (is_multi_line) {
            ++number_of_lines_comment;

            if (std::regex_match(line,
                                 std::regex(regex::MULTI_LINE_COMMENT_END))) {
                is_multi_line = false;
            }

            continue;
        }

        ++number_of_lines_code;
    }

    number_of_lines_total =
        number_of_lines_blank + number_of_lines_code + number_of_lines_comment;

    file_stats.path = path;
    file_stats.total = number_of_lines_total;
    file_stats.blank = number_of_lines_blank;
    file_stats.comment = number_of_lines_comment;
    file_stats.code = number_of_lines_code;

    return file_stats;
}

} // namespace cloc

} // namespace fima
