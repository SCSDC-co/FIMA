#pragma once

#include <filesystem>
#include <string>

namespace fima {

namespace tree {

void
start(const std::filesystem::path& path, std::string prefix, bool tui);

}

} // namespace fima
