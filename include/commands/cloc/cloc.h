#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void start(std::vector<fs::path> path);

}

} // namespace fima
