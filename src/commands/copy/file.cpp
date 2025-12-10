#include "../../../include/commands/copy/file.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace fima {

namespace copy {

void
file(fs::path source, fs::path destination)
{
    if (!fs::is_regular_file(source)) {
        std::cout << "The source file is a directory or does not exists! "
                  << source << '\n';
    }

    fs::copy_file(source, destination, fs::copy_options::overwrite_existing);

    std::cout << "File " << source << " copied to " << destination << '\n';
}

} // namespace copy

} // namespace fima
