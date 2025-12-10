#include "../../../../include/tui/commands/ls/ls_tui.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fima {

namespace ls {

void
tui(std::vector<std::string> dirs_vector, std::vector<std::string> files_vector)
{
    using namespace ftxui;

    std::vector<Element> dirs;
    std::vector<Element> files;

    size_t max_rows{ std::max(dirs_vector.size(), files_vector.size()) };

    for (size_t i = 0; i < max_rows; i++) {
        Element dir_cell{ i < dirs_vector.size()
                            ? text(" " + dirs_vector[i] + " ")
                            : text("") };

        Element file_cell{ i < files_vector.size()
                             ? text(" " + files_vector[i] + " ") |
                                 color(Color::White)
                             : text("") | color(Color::White) };

        dirs.push_back(dir_cell);
        files.push_back(file_cell);
    }

    Element dir_window =
      window(text(" DIRS ") | bold, vbox(dirs)) | color(Color::Green);

    Element files_window =
      window(text(" FILES ") | bold, vbox(files)) | color(Color::Green);

    auto main_box = hbox({
      dir_window,
      files_window,
    });

    auto document = main_box;
    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

} // namespace ls

} // namespace fima
