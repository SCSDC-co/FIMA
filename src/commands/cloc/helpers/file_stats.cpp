#include "../../../../include/commands/cloc/helpers/file_stats.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "../../../../include/helpers/get_directories_entries.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void FileStats::count_lines(const fs::path &path) {
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

void FileStats::process(const fs::directory_entry &path) {
    if (path.is_directory()) {
        std::vector<fs::directory_entry> path_entries =
            fima::helpers::get_directories_entries_recursive(path);

        for (auto it = path_entries.begin(); it != path_entries.end();) {
            if (it->is_directory()) {
                it = path_entries.erase(it);
            } else {
                ++it;
            }
        }

        for (const auto &entry : path_entries) {
            this->count_lines(entry);

            this->print();
        }
    } else if (path.is_regular_file()) {
        this->count_lines(path);

        this->print();
    }
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
    std::cout << '\n';
}

} // namespace cloc

} // namespace fima
