#include "../../../include/commands/create/directory.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace fima {
    namespace create {
        void dir(fs::path path) {
            if (fs::is_directory(path)) {
                std::cout << "This directory already exist! " << path << '\n';

                return;
            }

            fs::create_directories(path);

            std::cout << "Directory created at: " << path << '\n';
        }
    } // namespace create
} // namespace fima
