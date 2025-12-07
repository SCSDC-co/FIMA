#pragma once

#include <filesystem>

#include "./types.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

fima::cloc::LineStats cout_lines(fs::path path);

}

} // namespace fima
