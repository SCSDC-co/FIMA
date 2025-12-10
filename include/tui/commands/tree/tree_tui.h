#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

using namespace ftxui;

namespace fima {
namespace tree {
void
tui(std::string path_name,
    std::vector<ftxui::Element> tree_vector_tui,
    size_t number_of_dirs,
    size_t number_of_files);
}
} // namespace fima
