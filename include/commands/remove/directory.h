#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    namespace remove {
        void dir(fs::path path);
    }
} // namespace fima
