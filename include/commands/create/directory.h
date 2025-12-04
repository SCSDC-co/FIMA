#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fima {
    namespace create {
        void dir(const std::vector<fs::path> &paths);
    }
} // namespace fima
