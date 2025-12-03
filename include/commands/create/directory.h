#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    namespace create {
        void dir(fs::path path);
    }
} // namespace fima
