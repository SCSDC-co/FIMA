#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fima {
    namespace helpers {
        std::vector<fs::path> get_directories_entries(const fs::path &path);
    }
} // namespace fima
