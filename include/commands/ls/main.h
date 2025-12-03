#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    namespace ls {
        void start(fs::path path, bool tui);
    }
} // namespace fima
