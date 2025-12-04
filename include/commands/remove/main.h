#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fima {
    void remove(std::vector<fs::path> paths);
} // namespace fima
