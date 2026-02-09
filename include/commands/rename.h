#pragma once

#include <filesystem>

namespace fima {

void
rename(std::filesystem::path old_name, std::filesystem::path new_name);

}
