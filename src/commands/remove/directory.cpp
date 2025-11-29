#include "../../../include/commands/remove/directory.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void remove_dir(fs::path path) {
    if (!fs::is_directory(path)) {
        std::cout << "The directory doesn't exitst!" << path << '\n';

        return;
    }

    fs::remove_all(path);

    std::cout << "Directory removed: " << path << '\n';
}
