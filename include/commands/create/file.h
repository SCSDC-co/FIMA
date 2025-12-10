#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fima {
namespace create {
void
file(const std::vector<fs::path>& paths);
}
} // namespace fima
