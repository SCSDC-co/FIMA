#include "../../include/commands/remove.h"

#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace fima {

void remove(const std::vector<fs::path> &paths) {
    for (const auto &entry : paths) {
        if (!fs::exists(entry)) {
            std::cerr << "The path doesn't exists: " << entry.string()
                      << std::endl;

            continue;
        }

        try {
            fs::remove_all(entry);

            std::clog << "Item removed: " << entry.string() << '\n';
        } catch (const std::exception &ex) {
            std::cerr << "Failed to remove directory: " << entry.string()
                      << std::endl;
            std::cerr << ex.what() << std::endl;
        }
    }
}

} // namespace fima
