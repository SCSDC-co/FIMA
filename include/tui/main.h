#pragma once

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

namespace fs = std::filesystem;

namespace fima {
    namespace tui {
        void start_tui(fs::path path);
    }
} // namespace fima
