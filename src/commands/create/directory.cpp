#include "../../../include/commands/create/directory.h"

#include <exception>
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace fima {
    namespace create {
        void dir(const std::vector<fs::path> &paths) {
            for (const auto &entry : paths) {

                if (fs::is_directory(entry)) {
                    std::cerr << "This directory already exists: " << entry
                              << std::endl;

                    continue;
                }

                try {
                    fs::create_directories(entry);

                    std::clog << "Directory created at: " << entry << '\n';
                } catch (const std::exception &ex) {
                    std::cerr << "Failed to create the directory: " << entry
                              << std::endl;
                    std::cerr << ex.what() << std::endl;
                }
            }
        }
    } // namespace create
} // namespace fima
