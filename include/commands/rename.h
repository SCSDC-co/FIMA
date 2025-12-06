#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    void rename(fs::path old_name, fs::path new_name);
}
