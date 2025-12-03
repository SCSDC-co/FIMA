#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    namespace remove {
        void file(fs::path path);
    }
} // namespace fima
