#include <filesystem>
#include <iostream>
#include <vector>

#include "../../../include/helpers/get_directories_entries.h"
#include "../../../include/modules/ls/main.h"

namespace fs = std::filesystem;

void ls(fs::path path) {
    std::vector<fs::path> listOfTheDirectory = get_directories_entries(path);

    std::vector<std::string> dirDirs;

    std::vector<std::string> dirFiles;

    for (const fs::path &entry : listOfTheDirectory) {
        auto name{entry.filename().string()};

        if (fs::is_directory(entry)) {
            name += "/";

            dirDirs.push_back(name);
        } else {
            dirFiles.push_back(name);
        }
    }

    std::cout << "DIRS:" << std::endl;

    for (const auto &entry : dirDirs) {
        std::cout << entry << std::endl;
    }

    std::cout << "FILES:" << std::endl;

    for (const auto &entry : dirFiles) {
        std::cout << entry << std::endl;
    }
}
