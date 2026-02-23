/*
 * src/commands/tui.cpp
 * include/commands/tui.h
 *
 * The implementation of the `tree` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/tree.h"

#include <algorithm>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "logger.h"
#include "tui/commands/tree/tree_tui.h"
#include "utility/colors.h"

namespace fs = std::filesystem;

std::vector<Element> tree_vector_tui;

size_t number_of_files = 0;
size_t number_of_dirs  = 0;

std::vector<std::string> inner_pointers = { "├── ", "│   " };
std::vector<std::string> final_pointers = { "╰── ", "    " };

void
create_tree(std::string path, std::string prefix, bool tui)
{
    using namespace ftxui;

    std::vector<fs::directory_entry> entries;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().filename().string()[0] != '.') {
            entries.push_back(entry);
        }
    }

    sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
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
                std::cout << fima::colors::GREEN << prefix << pointers[0]
                          << entry.path().filename().string() << "/" << fima::colors::RESET
                          << std::endl;
            } else {
                std::cout << fima::colors::GREEN << prefix << pointers[0] << fima::colors::RESET
                          << entry.path().filename().string() << std::endl;
            }
        } else if (tui) {
            Element prefix_elem = text(prefix) | color(Color::Green);
            Element name_elem =
              text(entry.path().filename().string() + (entry.is_directory() ? "/ " : " "));
            Element first_pointer = text(pointers[0]);

            if (entry.is_directory()) {
                name_elem = name_elem | color(Color::Green);
            } else {
                name_elem = name_elem | color(Color::White);
            }

            Element total_string = hbox({ prefix_elem, first_pointer, name_elem });

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

void
start(const fs::path& path, std::string prefix, bool tui)
{
    if (!tui) {
        std::cout << fima::colors::GREEN << path.string()
                  << (path.string().back() == '/' ? "" : "/") << fima::colors::RESET << '\n';
    }

    create_tree(path.string(), prefix, tui);

    if (tui) {
        fima::tree::tui(path, tree_vector_tui, number_of_dirs, number_of_files);
    } else {
        std::cout << '\n';

        std::cout << fima::colors::GREEN << "Number of directories: " << fima::colors::RESET
                  << std::to_string(number_of_dirs) << '\n';
        std::cout << fima::colors::GREEN << "Number of files: " << fima::colors::RESET
                  << std::to_string(number_of_files) << '\n';
    }

    fima::logger::info(false, "tree", "Create tree of: {}", path.string());
    fima::logger::info(false, "tree", "Options:");
    fima::logger::info(false, "tree", "  Tui: {}", (tui ? "true" : "false"));
}

} // namespace tree

} // namespace fima
