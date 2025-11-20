#include "../../../include/modules/tree/main.h"

#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <vector>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fs = std::filesystem;

void tree(const fs::path &path) {
    using namespace ftxui;

    std::string path_name = path.string();

    std::vector<bool> last_flags;

    std::vector<Element> tree_vector;

    for (fs::recursive_directory_iterator it(path), end; it != end; ++it) {
        int depth = it.depth() + 1;
        auto name = it->path().filename().string();

        bool is_dir = it->is_directory();

        if (is_dir) {
            name += "/";
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
        prefix_str += (is_last ? "╰──" : "├──");

        Element prefix_elem = text(" " + prefix_str + " ");
        Element name_elem = text(name);

        if (is_dir) {
            name_elem = name_elem | color(Color::Green);
        } else {
            name_elem = name_elem | color(Color::White);
        }

        Element total_string = hbox({prefix_elem, name_elem});

        tree_vector.push_back(total_string);
    }

    Element main_box =
        window(text(" Tree: " + path_name + " ") | bold, vbox(tree_vector)) |
        color(Color::Green);

    auto document = main_box;
    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}
