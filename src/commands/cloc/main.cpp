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
    int number_of_lines{0};

    std::string line;
    std::ifstream file(path);

    while (std::getline(file, line)) {
        if (line.empty()) {
            ++number_of_lines_blank;

            continue;
        }

        ++number_of_lines;
    }

    number_of_lines_total = number_of_lines_blank + number_of_lines;

    std::cout << "Number of lines of " << path.string() << '\n';
    std::cout << "Total " << number_of_lines_total << '\n';
    std::cout << "Blank " << number_of_lines_blank << '\n';
    std::cout << "Other " << number_of_lines << '\n';
}

} // namespace cloc

} // namespace fima
