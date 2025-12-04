#include "../../../include/commands/create/file.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace fima {
    namespace create {
        void file(const std::vector<fs::path> &paths) {
            for (const auto &entry : paths) {
                if (fs::is_regular_file(entry)) {
                    std::cerr << "This file already exists: " << entry.string()
                              << '\n';

                    continue;
                }

                try {
                    std::ofstream outfile{entry};
                    outfile.close();

                    std::clog << "File created at: " << entry.string() << '\n';
                } catch (const std::exception &ex) {
                    std::cerr << "Failed to create file: " << entry.string()
                              << std::endl;
                    std::cerr << ex.what() << std::endl;
                }
            }
        }
    } // namespace create
} // namespace fima
