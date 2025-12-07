#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

struct LineStats {
    fs::path path;
    int total;
    int blank;
    int comment;
    int code;
};

} // namespace cloc

} // namespace fima
