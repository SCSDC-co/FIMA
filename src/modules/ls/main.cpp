#include "../../../include/modules/ls/main.h"

#include <filesystem>
#include <vector>

#include "../../../include/helpers/get_directories_entries.h"
#include "../../../include/tui/modules/ls/ls_tui.h"

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

    ls_tui(dirDirs, dirFiles);
}
