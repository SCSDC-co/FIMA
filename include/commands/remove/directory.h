#pragma once

#include <filesystem>

namespace fs = std::filesystem;

void remove_dir(fs::path path);
