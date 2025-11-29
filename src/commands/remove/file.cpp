#include "../../../include/commands/remove/file.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void remove_file(fs::path path) {
    if (!fs::is_regular_file(path)) {
        std::cout << "The file doesn't exitst!" << path << '\n';

        return;
    }

    fs::remove(path);

    std::cout << "File removed: " << path << '\n';
}
