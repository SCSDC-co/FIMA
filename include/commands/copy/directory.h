#pragma once

#include <filesystem>

namespace fs = std::filesystem;

void fima_copy_directory(fs::path source, fs::path destination);
