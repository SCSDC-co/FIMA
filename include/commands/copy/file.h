#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    namespace copy {
        void file(fs::path source, fs::path destination);
    }
} // namespace fima
