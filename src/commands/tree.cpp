/*
 * src/commands/tree.cpp
 * include/commands/tree.h
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

#include "config.h"
#include "fs/get_directories_entries.h"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "git/GitRepo.h"
#include "logger.h"
#include "options.h"
#include "tui/commands/tree/tree_tui.h"
#include "utility/colors.h"
#include "utility/regex.h"

std::vector<Element> tree_vector_tui;

std::vector<std::string> inner_pointers = { "├── ", "│   " };
std::vector<std::string> final_pointers = { "╰── ", "    " };

size_t number_of_files = 0;
size_t number_of_dirs  = 0;

void
create_tree(const std::filesystem::directory_entry& path,
            const fima::git::GitRepo& repo,
            const fima::options::tree_options& options,
            const std::string& prefix)
{
    using namespace ftxui;

    std::vector<std::filesystem::directory_entry> entries;

    if (options.gitignore) {
        entries = fima::fs::get_directories_entries(path, repo, options.all);
    } else {
        entries = fima::fs::get_directories_entries_no_git(path, options.all);
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

    std::vector<std::filesystem::directory_entry> paths_sanitized;

    for (const auto& entry : entries) {
        if (fima::helpers::regex::matches_any_regex(entry.path().filename().string(),
                                                    fima::config::DEFAULT_DIRS_TO_IGNORE)) {
            continue;
        }

        paths_sanitized.push_back(entry);
    }

    for (size_t index = 0; index < paths_sanitized.size(); index++) {
        std::filesystem::directory_entry entry = paths_sanitized[index];
        std::vector<std::string> pointers =
          (index == paths_sanitized.size() - 1 ? final_pointers : inner_pointers);

        if (!options.tui) {
            if (entry.is_directory()) {
                std::cout << fima::colors::GREEN << prefix << pointers[0]
                          << entry.path().filename().string() << "/" << fima::colors::RESET
                          << std::endl;
            } else {
                std::cout << fima::colors::GREEN << prefix << pointers[0] << fima::colors::RESET
                          << entry.path().filename().string() << std::endl;
            }
        } else if (options.tui) {
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

            create_tree(entry, repo, options, prefix + pointers[1]);
        }
    }
}

namespace fima {

namespace tree {

void
start(const std::filesystem::directory_entry& path,
      const fima::git::GitRepo& repo,
      const fima::options::tree_options& options)
{
    if (!options.tui) {
        std::cout << fima::colors::GREEN << path.path().string()
                  << (path.path().string().back() == '/' ? "" : "/") << fima::colors::RESET << '\n';
    }

    create_tree(path, repo, options, options.prefix);

    if (options.tui) {
        fima::tree::tui(path, tree_vector_tui, number_of_dirs, number_of_files);
    } else if (options.verbose) {
        std::cout << '\n';

        std::cout << fima::colors::GREEN << "Number of directories: " << fima::colors::RESET
                  << std::to_string(number_of_dirs) << '\n';
        std::cout << fima::colors::GREEN << "Number of files: " << fima::colors::RESET
                  << std::to_string(number_of_files) << '\n';
    }

    fima::logger::info(false, "tree", "Create tree of: {}", path.path().string());
    fima::logger::info(false, "tree", "Options:");
    fima::logger::info(false, "tree", "  Tui: {}", (options.tui ? "true" : "false"));
}

} // namespace tree

} // namespace fima
