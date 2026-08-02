/*
 * src/commands/ls/ls.cpp
 * include/commands/ls/ls.h
 *
 * The implementation of the `ls` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/ls/ls.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <glob/glob.hpp>
#include <iostream>
#include <vector>

#include "CLI/CLI.hpp"
#include "commands/ls/helpers/printer.h"
#include "fs/DirectoryItem.h"
#include "fs/get_directories_entries.h"
#include "git/GitRepo.h"
#include "options.h"
#include "theme.h"

namespace fima {

namespace commands {

void
ls(const fima::git::GitRepo& repo, fima::options::ls_options& options)
{
    auto to_lower = [=](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    };

    std::sort(options.paths.begin(),
              options.paths.end(),
              [to_lower](const std::filesystem::path& a, const std::filesystem::path& b) {
                  if (std::filesystem::is_directory(a) != std::filesystem::is_directory(b)) {
                      return !std::filesystem::is_directory(a);
                  }

                  return to_lower(a.filename().string()) < to_lower(b.filename().string());
              });

    for (auto it = options.paths.begin(); it != options.paths.end(); ++it) {
        auto item = *it;

        std::vector<std::filesystem::path> list_of_the_directory;

        if (std::filesystem::is_directory(item)) {
            list_of_the_directory.append_range(
              fima::fs::get_directories_entries_no_git(item, options.all));
        } else {
            list_of_the_directory = glob::rglob(item.string());
        }

        if (options.directory_first) {
            std::sort(list_of_the_directory.begin(),
                      list_of_the_directory.end(),
                      [to_lower](const std::filesystem::path& a, const std::filesystem::path& b) {
                          if (std::filesystem::is_directory(a) !=
                              std::filesystem::is_directory(b)) {
                              return std::filesystem::is_directory(a);
                          }

                          return to_lower(a.filename().string()) < to_lower(b.filename().string());
                      });
        } else if (options.directory_last) {
            std::sort(list_of_the_directory.begin(),
                      list_of_the_directory.end(),
                      [to_lower](const std::filesystem::path& a, const std::filesystem::path& b) {
                          if (std::filesystem::is_directory(a) !=
                              std::filesystem::is_directory(b)) {
                              return !std::filesystem::is_directory(a);
                          }

                          return to_lower(a.filename().string()) < to_lower(b.filename().string());
                      });
        } else {
            std::sort(list_of_the_directory.begin(),
                      list_of_the_directory.end(),
                      [to_lower](const std::filesystem::path& a, const std::filesystem::path& b) {
                          return to_lower(a.filename().string()) < to_lower(b.filename().string());
                      });
        }

        std::vector<fima::fs::DirectoryItem> items{};

        for (const std::filesystem::path& item : list_of_the_directory) {
            items.emplace_back(item);
        }

        // remove items that are invalid
        items.erase(
          std::remove_if(items.begin(),
                         items.end(),
                         [](const fima::fs::DirectoryItem& item) { return !item.is_valid(); }),
          items.end());

        if (options.paths.size() > 1 && std::filesystem::is_directory(item)) {
            std::cout << '\n'
                      << fima::theme::theme.primary << item.string() << '/'
                      << fima::theme::Color::reset << '\n';
        }

        if (options.long_output) {
            ls::helpers::print_long(items, options.icons, options.verbose, options.headers);
        } else if (options.one_line) {
            ls::helpers::print_one_line(items, options.icons);
        } else {
            ls::helpers::print_normal(items, options.icons);
        }
    }
}

void
setup_ls(CLI::App& app, const fima::git::GitRepo& repo, fima::options::ls_options& options)
{
    CLI::App* subcmd =
      app.add_subcommand("ls", "Print the content of the directory like the ls command")
        ->configurable(false);

    subcmd->add_option("paths", options.paths, "The paths to work on (default current directory)")
      ->configurable(false);

    subcmd->add_flag("-i,--icons", options.icons, "Put an icon next to the name of the item")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-a,--all", options.all, "Will also count the dotfiles")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-1,--one-line", options.one_line, "Display one entry per line")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd
      ->add_flag(
        "--group-directories-first", options.directory_first, "List directories before files")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd
      ->add_flag("--group-directories-last", options.directory_last, "List directories after files")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-l,--long", options.long_output, "Display the file metadata")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-v,--verbose", options.verbose, "Display the number of directories and files")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true)
      ->group("LONG OUTPUT OPTIONS");

    subcmd->add_flag("-H,--headers", options.headers, "Add a header to each column")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true)
      ->group("LONG OUTPUT OPTIONS");

    subcmd->usage("fima [PATH] ls [OPTIONS]");

    subcmd->callback([&]() { fima::commands::ls(repo, options); });
}

} // namespace commands

} // namespace fima
