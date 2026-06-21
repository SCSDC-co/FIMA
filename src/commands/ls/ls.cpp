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
#include <vector>

#include "CLI/CLI.hpp"
#include "commands/ls/helpers/printer.h"
#include "fs/DirectoryItem.h"
#include "fs/get_directories_entries.h"
#include "git/GitRepo.h"
#include "options.h"

namespace fima {

namespace commands {

void
ls(const std::filesystem::path& path,
   const fima::git::GitRepo& repo,
   const fima::options::ls_options& options)
{
    std::vector<std::filesystem::directory_entry> list_of_the_directory;

    list_of_the_directory = fima::fs::get_directories_entries_no_git(path, options.all);

    auto to_lower = [=](std::string s) {
        std::transform(
          s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    };

    // what was I thinking when I did the 2 vector thing? Sorting the vector is much better
    std::sort(list_of_the_directory.begin(),
              list_of_the_directory.end(),
              [to_lower](const std::filesystem::directory_entry& a,
                         const std::filesystem::directory_entry& b) {
                  if (a.is_directory() != b.is_directory()) {
                      return a.is_directory();
                  }

                  return to_lower(a.path().filename().string()) <
                         to_lower(b.path().filename().string());
              });

    std::vector<fima::fs::DirectoryItem> items{};

    for (const std::filesystem::directory_entry& item : list_of_the_directory) {
        items.emplace_back(item);
    }

    // remove items that are invalid
    items.erase(
      std::remove_if(items.begin(),
                     items.end(),
                     [](const fima::fs::DirectoryItem& item) { return !item.is_valid(); }),
      items.end());

    if (options.long_output) {
        ls::helpers::print_long(items, options.icons, options.verbose, options.headers);
    } else {
        ls::helpers::print_normal(items, options.icons);
    }
}

void
setup_ls(CLI::App& app,
         const std::filesystem::directory_entry& path,
         const fima::git::GitRepo& repo,
         fima::options::ls_options& options)
{
    CLI::App* subcmd =
      app.add_subcommand("ls", "Print the content of the directory like the ls command")
        ->configurable(false);

    subcmd->add_flag("-i,--icons", options.icons, "Put an icon next to the name of the item")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-a,--all", options.all, "Will also count the dotfiles")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-l,--long", options.long_output, "Display the file metadata")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd
      ->add_flag("-v,--verbose",
                 options.verbose,
                 "Display the number of directories and files (only works with long output)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd
      ->add_flag("-H,--headers",
                 options.headers,
                 "Add a header to each column (only works with long output)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->usage("fima ls [OPTIONS]");

    subcmd->callback([&]() { fima::commands::ls(path, repo, options); });
}

} // namespace commands

} // namespace fima
