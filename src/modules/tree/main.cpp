#include "../../../include/modules/tree/main.h"

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fs = std::filesystem;

/*  This file will contain both the core logic of the tree but also the TUI
 *  logic, i'm too lazy to put them in 2 different files XD
 */

void tree(const fs::path &path, bool tui) {
    using namespace ftxui;

    int number_of_files = 0;
    int number_of_dirs = 0;

    std::string path_name = path.string() + "/";

    std::vector<bool> last_flags;

    std::vector<std::string> tree_vector;

    std::vector<Element> tree_vector_tui;

    for (fs::recursive_directory_iterator it(path), end; it != end; ++it) {
        int depth = it.depth() + 1;
        auto name = it->path().filename().string();

        bool is_dir = it->is_directory();

        if (is_dir) {
            name += "/";

            number_of_dirs++;
        }

        if (fs::is_regular_file(*it)) {
            number_of_files++;
        }

        fs::path parent = it->path().parent_path();
        size_t siblings_count = std::distance(fs::directory_iterator(parent),
                                              fs::directory_iterator());

        size_t index = 0;

        for (auto &sibling : fs::directory_iterator(parent)) {
            if (sibling.path() == it->path()) {
                break;
            }

            ++index;
        }

        bool is_last = (index == siblings_count - 1);

        if (depth > (int)last_flags.size()) {
            last_flags.push_back(is_last);
        } else {
            last_flags[depth - 1] = is_last;
        }

        std::string prefix_str;
        for (int d = 1; d < depth; ++d) {
            prefix_str += (last_flags[d - 1] ? "    " : "│   ");
        }

        prefix_str += (is_last ? "╰── " : "├── ");

        if (!tui) {
            tree_vector.push_back(prefix_str + name);

            continue;
        }

        Element prefix_elem = text(prefix_str);
        Element name_elem = text(name + " ");

        if (is_dir) {
            name_elem = name_elem | color(Color::Green);
        } else {
            name_elem = name_elem | color(Color::White);
        }

        Element total_string = hbox({prefix_elem, name_elem});

        tree_vector_tui.push_back(total_string);
    }

    if (tui) {
        Element main_box =
            window(
                text(" Tree: " + path_name + " ") | bold,
                vbox(

                    hbox(text(" "), vbox(tree_vector_tui)),

                    text(""),

                    vbox(

                        hbox(text(" Number of directories: ") |
                                 color(Color::Green),
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
    } else {
        std::cout << path_name << '\n';

        for (const auto &entry : tree_vector) {
            std::cout << entry << '\n';
        }

        std::cout << '\n';

        std::cout << "Number of directories: " << number_of_dirs << '\n';
        std::cout << "Number of files: " << number_of_files << '\n';
    }
}
