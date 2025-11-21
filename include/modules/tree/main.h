#pragma once

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void tree(const fs::path &path, bool tui);
