#include "../../../../include/commands/cloc/helpers/file_stats.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void FileStats::process(fs::path path) {
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
            std::regex_match(line,
                             std::regex(this->MULTI_LINE_COMMENT_START))) {

            is_multi_line = true;
            ++number_of_lines_comment;

            continue;
        }

        if (is_multi_line) {
            ++number_of_lines_comment;

            if (std::regex_match(line,
                                 std::regex(this->MULTI_LINE_COMMENT_END))) {
                is_multi_line = false;
            }

            continue;
        }

        ++number_of_lines_code;
    }

    number_of_lines_total =
        number_of_lines_blank + number_of_lines_code + number_of_lines_comment;

    this->code = number_of_lines_code;
    this->blank = number_of_lines_blank;
    this->total = number_of_lines_total;
    this->comment = number_of_lines_comment;
    this->path = path;
}

void FileStats::print() {
    char dash{'-'};
    auto number_of_dashes{std::to_string(this->total).size()};

    std::cout << "Number of lines of " << this->path.string() << '\n';
    std::cout << "Comment " << this->comment << '\n';
    std::cout << "Blank   " << this->blank << '\n';
    std::cout << "Code    " << this->code << '\n';
    std::cout << "--------" << std::string(number_of_dashes, dash) << '\n';
    std::cout << "Total   " << this->total << '\n';
}

} // namespace cloc

} // namespace fima
