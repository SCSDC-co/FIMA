#include "../../../include/commands/remove/directory.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace fima {
    namespace remove {
        void dir(fs::path path) {
            if (!fs::is_directory(path)) {
                std::cout << "The directory doesn't exitst!" << path << '\n';

                return;
            }

            fs::remove_all(path);

            std::cout << "Directory removed: " << path << '\n';
        }
    } // namespace remove
} // namespace fima
