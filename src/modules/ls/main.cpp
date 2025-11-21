#include "../../../include/modules/ls/main.h"

#include <filesystem>
#include <vector>

#include "../../../include/helpers/get_directories_entries.h"
#include "../../../include/tui/modules/ls/ls_tui.h"

namespace fs = std::filesystem;

void ls(fs::path path, bool tui) {
    std::vector<fs::path> listOfTheDirectory{get_directories_entries(path)};

    std::vector<std::string> vector_directories;

    std::vector<std::string> vector_files;

    for (const fs::path &entry : listOfTheDirectory) {
        auto name{entry.filename().string()};

        if (fs::is_directory(entry)) {
            name += "/";

            vector_directories.push_back(name);
        } else {
            vector_files.push_back(name);
        }
    }

    if (tui) {
        ls_tui(vector_directories, vector_files);
    } else {
        std::cout << "DIRS" << '\n';

        for (const auto &entry : vector_directories) {
            std::cout << entry << '\n';
        }

        std::cout << '\n';

        std::cout << "FILES" << '\n';

        for (const auto &entry : vector_files) {
            std::cout << entry << '\n';
        }
    }
}
