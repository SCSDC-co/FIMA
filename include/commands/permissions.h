#pragma once

#include <filesystem>
#include <vector>

namespace fima {

void
get_perms(const std::vector<std::filesystem::path>& paths);

}
