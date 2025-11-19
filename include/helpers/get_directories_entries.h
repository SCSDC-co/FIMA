#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> get_directories_entries(const fs::path &path);
