#pragma once

#include <filesystem>

namespace fs = std::filesystem;

void get_perms(fs::path path);
