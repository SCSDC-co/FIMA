#ifndef FIMA_TUI_H
#define FIMA_TUI_H

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

#include "../../include/helpers/get_directories_entries.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fs = std::filesystem;

void start_tui(fs::path path);

#endif
