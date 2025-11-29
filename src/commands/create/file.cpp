#include "../../../include/commands/create/file.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void create_file(fs::path path) {
    if (fs::is_regular_file(path)) {
        std::cout << "This file already exists! " << path << '\n';

        return;
    }

    std::ofstream outfile{path};
    outfile.close();

    std::cout << "File created at: " << path << '\n';
}
