#include "../../../include/commands/cloc/main.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void cout_lines(fs::path path) {
    int number_of_lines{0};

    std::string line;
    std::ifstream file(path);

    while (std::getline(file, line)) {
        ++number_of_lines;
    }

    std::cout << "Number of lines of " << path.string() << " - "
              << number_of_lines << '\n';
}

} // namespace cloc

} // namespace fima
