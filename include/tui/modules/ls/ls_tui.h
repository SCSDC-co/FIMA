#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

void ls_tui(std::vector<std::string> dirs_vector,
            std::vector<std::string> files_vector);
