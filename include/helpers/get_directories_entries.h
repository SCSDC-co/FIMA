#ifndef FIMA_LIST_DIRECTORIES_H
#define FIMA_LIST_DIRECTORIES_H

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<fs::path> get_directories_entries(const fs::path &path);

#endif
