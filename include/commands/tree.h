#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace fima {
namespace tree {
void
start(const fs::path& path, std::string prefix, bool tui);
}
} // namespace fima
