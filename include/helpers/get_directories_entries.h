#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fima {

namespace helpers {

std::vector<fs::directory_entry> get_directories_entries(const fs::path &path);

std::vector<fs::directory_entry>
get_directories_entries_recursive(const fs::path &path);

} // namespace helpers

} // namespace fima
