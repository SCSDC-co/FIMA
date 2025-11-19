#include "../../include/helpers/get_directories_entries.h"

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> get_directories_entries(const fs::path &path) {
    std::vector<fs::path> directories;

    for (const fs::path &entry : std::filesystem::directory_iterator(path)) {
        std::string name{entry.filename().string()};

        if (name[0] == '.') {
            continue;
        }

        directories.push_back(entry);
    }

    return directories;
}
