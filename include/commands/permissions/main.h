#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {
    void get_perms(fs::path path);
}
