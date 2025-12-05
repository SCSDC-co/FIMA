#include "../../../../include/tui/commands/tree/tree_tui.h"

#include <cstdio>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

using namespace ftxui;

namespace fima {
    namespace tree {
        void tui(std::string path_name,
                 std::vector<ftxui::Element> tree_vector_tui,
                 size_t number_of_dirs, size_t number_of_files) {
            Element main_box =
                window(text(" Tree: " + path_name +
                            (path_name.back() == '/' ? " " : "/ ")) |
                           bold,
                       vbox(

                           hbox(text(" "), vbox(tree_vector_tui)),

                           filler(),

                           separator(),

                           vbox(

                               hbox(text(" Number of directories: ") |
                                        color(Color::Green),
                                    text(std::to_string(number_of_dirs)) |
                                        color(Color::White)),

                               hbox(text(" Number of files: ") |
                                        color(Color::Green),
                                    text(std::to_string(number_of_files)) |
                                        color(Color::White))

                                   ))) |
                color(Color::Green);

            auto document = main_box;
            auto screen =
                Screen::Create(Dimension::Fit(document), Dimension::Full());
            Render(screen, document);
            screen.Print();
            getchar();
        }
    } // namespace tree
} // namespace fima
