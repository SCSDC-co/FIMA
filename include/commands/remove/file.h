#pragma once

#include <filesystem>

namespace fs = std::filesystem;

void remove_file(fs::path path);
