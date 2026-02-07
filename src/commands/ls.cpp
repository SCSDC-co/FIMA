#include "../../include/commands/ls.h"

#include <filesystem>
#include <iostream>
#include <vector>

#include "../../include/helpers/colors.h"
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
        std::cout << fima::colors::GREEN << "DIRS" << fima::colors::RESET
                  << '\n';

        for (const auto& entry : vector_directories) {
            std::cout << fima::colors::GREEN << entry << fima::colors::RESET
                      << '\n';
        }

        std::cout << '\n';

        std::cout << fima::colors::GREEN << "FILES" << fima::colors::RESET
                  << '\n';

        for (const auto& entry : vector_files) {
            std::cout << entry << '\n';
        }
    }
}

} // namespace ls

} // namespace fima
