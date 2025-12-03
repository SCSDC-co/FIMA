#include "../../../include/commands/copy/directory.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace fima {
    namespace copy {
        void directory(fs::path source, fs::path destination) {
            if (!fs::is_directory(source)) {
                std::cout
                    << "The source directory is a file or does not exists! "
                    << source << '\n';
            }

            if (!fs::is_directory(destination)) {
                fs::create_directory(destination);
            }

            fs::copy(source, destination,
                     fs::copy_options::overwrite_existing |
                         fs::copy_options::recursive);

            std::cout << "Directory " << source << " copied to " << destination
                      << '\n';
        }
    } // namespace copy
} // namespace fima
