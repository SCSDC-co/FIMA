#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fima {
void
get_perms(const std::vector<fs::path>& paths);
}
