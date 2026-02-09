#pragma once

#include <filesystem>
#include <vector>

namespace fima {

void
remove(const std::vector<std::filesystem::path>& paths);

} // namespace fima
