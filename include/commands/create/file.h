#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    namespace create {
        void file(fs::path path);
    }
} // namespace fima
