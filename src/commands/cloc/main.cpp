#include "../../../include/commands/cloc/main.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "../../../include/commands/cloc/helpers/is_comment.h"

namespace fs = std::filesystem;

#define MULTI_LINE_COMMENT_START "^ *(/\\*|<!--|\\\{-|\"\"\"|'''|--\\\[\\\[).*$"
#define MULTI_LINE_COMMENT_END "^ *(\\*/|-->|-}|\"\"\"|'''|\\]\\]).*$"

namespace fima {

namespace cloc {

void cout_lines(fs::path path) {
    int number_of_lines_total{0};
    int number_of_lines_blank{0};
    int number_of_lines_code{0};
    int number_of_lines_comment{0};

    bool is_multi_line{false};

    std::string line;
    std::ifstream file(path);

    while (std::getline(file, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            ++number_of_lines_blank;

            continue;
        }

        if (is_comment(line)) {
            ++number_of_lines_comment;
        }

        if (!is_multi_line &&
            std::regex_match(line, std::regex(MULTI_LINE_COMMENT_START))) {

            is_multi_line = true;
            ++number_of_lines_comment;

            continue;
        }

        if (is_multi_line) {
            ++number_of_lines_comment;

            if (std::regex_match(line, std::regex(MULTI_LINE_COMMENT_END))) {
                is_multi_line = false;
            }

            continue;
        }

        ++number_of_lines_code;
    }

    number_of_lines_total =
        number_of_lines_blank + number_of_lines_code + number_of_lines_comment;

    std::cout << "Number of lines of " << path.string() << '\n';
    std::cout << "Total " << number_of_lines_total << '\n';
    std::cout << "Blank " << number_of_lines_blank << '\n';
    std::cout << "Code " << number_of_lines_code << '\n';
    std::cout << "Comment " << number_of_lines_comment << '\n';
}

} // namespace cloc

} // namespace fima
