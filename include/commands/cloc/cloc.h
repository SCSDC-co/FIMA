#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void cout_lines(fs::path path);

}

} // namespace fima
