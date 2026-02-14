#pragma once

#include <filesystem>
#include <vector>

namespace fima {

namespace helpers {

std::vector<std::filesystem::directory_entry>
get_directories_entries(const std::filesystem::path& path);

std::vector<std::filesystem::path>
get_directories_entries_recursive(const std::filesystem::path& path,
                                  const bool& ignore_directories = false);

} // namespace helpers

} // namespace fima
