#include "../../include/commands/ls.h"

#include <filesystem>
#include <iostream>
#include <vector>

#include "../../include/helpers/get_directories_entries.h"
#include "../../include/tui/commands/ls/ls_tui.h"

namespace fs = std::filesystem;

namespace fima {

namespace ls {

void
start(fs::path path, bool tui)
{
    std::vector<fs::directory_entry> listOfTheDirectory{
        fima::helpers::get_directories_entries(path)
    };

    std::vector<std::string> vector_directories;

    std::vector<std::string> vector_files;

    std::string green = "\033[32m";
    std::string reset = "\033[0m";

    for (const fs::path& entry : listOfTheDirectory) {
        auto name{ entry.filename().string() };

        if (fs::is_directory(entry)) {
            name += "/";

            vector_directories.push_back(name);
        } else {
            vector_files.push_back(name);
        }
    }

    if (tui) {
        fima::ls::tui(vector_directories, vector_files);
    } else {
        std::cout << green << "DIRS" << reset << '\n';

        for (const auto& entry : vector_directories) {
            std::cout << green << entry << reset << '\n';
        }

        std::cout << '\n';

        std::cout << green << "FILES" << reset << '\n';

        for (const auto& entry : vector_files) {
            std::cout << entry << '\n';
        }
    }
}

} // namespace ls

} // namespace fima
