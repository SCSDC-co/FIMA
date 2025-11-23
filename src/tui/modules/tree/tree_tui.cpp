#include "../../../../include/tui/modules/tree/tree_tui.h"

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
              size_t number_of_dirs, size_t number_of_files) {
    Element main_box =
        window(
            text(" Tree: " + path_name + "/ ") | bold,
            vbox(

                hbox(text(" "), vbox(tree_vector_tui)),

                text(""),

                vbox(

                    hbox(text(" Number of directories: ") | color(Color::Green),
                         text(std::to_string(number_of_dirs)) |
                             color(Color::White)),

                    hbox(text(" Number of files: ") | color(Color::Green),
                         text(std::to_string(number_of_files)) |
                             color(Color::White))

                        ))) |
        color(Color::Green);

    auto document = main_box;
    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << '\n';
}
