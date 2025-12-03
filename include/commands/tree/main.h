#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void tree(const fs::path &path, std::string prefix, bool tui);
