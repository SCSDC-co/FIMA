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
#include "options.h"
#include "theme.h"
#include "tui/commands/tree/tree_tui.h"
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
        if (fima::utility::regex::matches_any_regex(entry.path().filename().string(),
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
                std::cout << fima::theme::theme.primary << prefix << pointers[0]
                          << entry.path().filename().string() << "/" << fima::theme::theme.secondary
                          << fima::theme::Color::reset << std::endl;
            } else {
                std::cout << fima::theme::theme.primary << prefix << pointers[0]
                          << fima::theme::theme.secondary << entry.path().filename().string()
                          << fima::theme::Color::reset << std::endl;
            }
        } else {
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

void
create_tree(const std::filesystem::directory_entry& path,
            const fima::git::GitRepo& repo,
            const fima::options::tree_options& options)
{
    if (!options.tui) {
        std::cout << fima::theme::theme.primary << path.path().string()
                  << (path.path().string().back() == '/' ? "" : "/") << fima::theme::theme.secondary
                  << fima::theme::Color::reset << '\n';
    }

    create_tree(path, repo, options, options.prefix);

    if (options.tui) {
        fima::tree::tui(path, tree_vector_tui, number_of_dirs, number_of_files);
    } else if (options.verbose) {
        std::cout << '\n';

        std::cout << fima::theme::theme.primary
                  << "Number of directories: " << fima::theme::theme.secondary
                  << std::to_string(number_of_dirs) << fima::theme::Color::reset << '\n';
        std::cout << fima::theme::theme.primary
                  << "Number of files: " << fima::theme::theme.secondary
                  << std::to_string(number_of_files) << fima::theme::Color::reset << '\n';
    }
}

namespace fima {

namespace commands {

void
setup_tree(CLI::App& app,
           const std::filesystem::directory_entry& path,
           const fima::git::GitRepo& repo,
           fima::options::tree_options& options)
{
    CLI::App* subcmd =
      app.add_subcommand("tree", "Print the tree of the directory like the tree command")
        ->configurable(false);

    subcmd->add_flag("-a,--all", options.all, "Show dotfiles")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd
      ->add_flag(
        "-G,--no-gitignore", [&](int) { options.gitignore = false; }, "Ignore .gitignore")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-v,--verbose", options.verbose, "Verbose output")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->usage("fima [PATH] tree [OPTIONS]");

    // when calling through the CLI it shouldn't be use a TUI
    options.tui = false;

    subcmd->callback([&]() { create_tree(path, repo, options); });
}

} // namespace commands

} // namespace fima
