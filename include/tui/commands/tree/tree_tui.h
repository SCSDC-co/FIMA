#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

using namespace ftxui;

void tree_tui(std::string path_name,
              std::vector<ftxui::Element> tree_vector_tui,
              size_t number_of_dirs, size_t number_of_files);
