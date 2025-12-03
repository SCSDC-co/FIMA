#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    namespace copy {
        void directory(fs::path source, fs::path destination);
    }
} // namespace fima
