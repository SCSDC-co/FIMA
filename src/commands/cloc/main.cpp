#include "../../../include/commands/cloc/main.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void cout_lines(fs::path path) {
    int number_of_lines_total{0};
    int number_of_lines_blank{0};
    int number_of_lines_code{0};

    std::string line;
    std::ifstream file(path);

    while (std::getline(file, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            ++number_of_lines_blank;

            continue;
        }

        ++number_of_lines_code;
    }

    number_of_lines_total = number_of_lines_blank + number_of_lines_code;

    std::cout << "Number of lines of " << path.string() << '\n';
    std::cout << "Total " << number_of_lines_total << '\n';
    std::cout << "Blank " << number_of_lines_blank << '\n';
    std::cout << "Code " << number_of_lines_code << '\n';
}

} // namespace cloc

} // namespace fima
