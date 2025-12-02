#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void fima_copy_file(fs::path source, fs::path destination) {
    if (!fs::is_regular_file(source)) {
        std::cout << "The source file is a directory or does not exists! "
                  << source << '\n';
    }

    fs::copy_file(source, destination, fs::copy_options::overwrite_existing);

    std::cout << "File " << source << " copied to " << destination << '\n';
}
