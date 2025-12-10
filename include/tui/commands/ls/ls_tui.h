#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

namespace fima {
namespace ls {
void
tui(std::vector<std::string> dirs_vector,
    std::vector<std::string> files_vector);
}
} // namespace fima
