#include "../../../include/commands/tree/main.h"

#include <algorithm>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "../../../include/tui/commands/tree/tree_tui.h"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fs = std::filesystem;

std::vector<Element> tree_vector_tui;

size_t number_of_files = 0;
size_t number_of_dirs = 0;

std::vector<std::string> inner_pointers = {"├── ", "│   "};
std::vector<std::string> final_pointers = {"╰── ", "    "};

std::string green = "\033[32m";
std::string reset = "\033[0m";

void create_tree(std::string path, std::string prefix, bool tui) {
    using namespace ftxui;

    std::vector<fs::directory_entry> entries;

    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.path().filename().string()[0] != '.') {
            entries.push_back(entry);
        }
    }

    sort(entries.begin(), entries.end(), [](auto &a, auto &b) {
        if (a.is_directory() && !b.is_directory()) {
            return true;
        }

        if (!a.is_directory() && b.is_directory()) {
            return false;
        }

        return a.path().filename() < b.path().filename();
    });

    for (size_t index = 0; index < entries.size(); index++) {
        fs::directory_entry entry = entries[index];
        std::vector<std::string> pointers =
            (index == entries.size() - 1 ? final_pointers : inner_pointers);

        if (!tui) {
            if (entry.is_directory()) {
                std::cout << green << prefix << pointers[0]
                          << entry.path().filename().string() << "/" << reset
                          << std::endl;
            } else {
                std::cout << green << prefix << pointers[0] << reset
                          << entry.path().filename().string() << std::endl;
            }
        } else if (tui) {
            Element prefix_elem = text(prefix) | color(Color::Green);
            Element name_elem = text(entry.path().filename().string() +
                                     (entry.is_directory() ? "/ " : " "));
            Element first_pointer = text(pointers[0]);

            if (entry.is_directory()) {
                name_elem = name_elem | color(Color::Green);
            } else {
                name_elem = name_elem | color(Color::White);
            }

            Element total_string =
                hbox({prefix_elem, first_pointer, name_elem});

            tree_vector_tui.push_back(total_string);
        }

        if (!entry.is_directory()) {
            number_of_files++;
        } else if (entry.is_directory()) {
            number_of_dirs++;
            create_tree(entry.path(), prefix + pointers[1], tui);
        }
    }
}
namespace fima {
    namespace tree {
        void start(const fs::path &path, std::string prefix, bool tui) {
            if (!tui) {
                std::cout << green << path.string()
                          << (path.string().back() == '/' ? "" : "/") << reset
                          << '\n';
            }

            create_tree(path.string(), prefix, tui);

            if (tui) {
                fima::tree::tui(path, tree_vector_tui, number_of_dirs,
                                number_of_files);
            } else {
                std::cout << '\n';

                std::cout << green << "Number of directories: " << reset
                          << std::to_string(number_of_dirs) << '\n';
                std::cout << green << "Number of files: " << reset
                          << std::to_string(number_of_files) << '\n';
            }
        }
    } // namespace tree
} // namespace fima
