/*
 * src/commands/info.cpp
 * include/commands/info.h
 *
 * A command that gives information about a file/directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/info/info.h"

#include <filesystem>
#include <iostream>

#include "CLI/CLI.hpp"
#include "commands/info/directory.h"
#include "commands/info/file.h"
#include "commands/info/git.h"
#include "theme.h"

void
_info(const fima::options::info_options& options,
      const std::filesystem::directory_entry& path,
      fima::git::GitRepo& repo)
{
    if (!std::filesystem::exists(path)) {
        std::cerr << fima::theme::theme.error
                  << "The path doesn't exist: " << fima::theme::theme.secondary
                  << path.path().string() << fima::theme::Color::reset << '\n';

        return;
    }

    std::filesystem::directory_entry can_path(std::filesystem::canonical(path));

    if (options.git) {
        repo.change_repo_path(can_path);

        fima::info::git(options, repo);

        return;
    }

    if (options.file_path.path().empty()) {
        fima::info::dir(can_path, options.verbose, repo);
    } else {
        fima::info::file(options.file_path);
    }
}

namespace fima {

namespace commands {

void
setup_info(CLI::App& app,
           const std::filesystem::directory_entry& path,
           fima::git::GitRepo& repo,
           fima::options::info_options& options)
{
    CLI::App* subcmd = app.add_subcommand("info", "Display information about a file or directory")
                         ->configurable(false);

    subcmd->get_formatter()->label("POSITIONALS", "FILE PATH");

    subcmd
      ->add_option("file", options.file_path, "The path of the file you want to get the info on")
      ->configurable(false)
      ->check(CLI::ExistingFile);

    subcmd->add_flag("-v,--verbose", options.verbose, "Display additional information")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true)
      ->group("DIRECTORY OUTPUT OPTIONS");

    subcmd->add_flag("-g,--git", options.git, "Display git information instead of file/directory")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(false);

    subcmd->add_flag("-t,--tags", options.tags, "Show all the repo tags")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true)
      ->group("GIT OUTPUT OPTIONS");

    subcmd->add_flag("-r,--remotes", options.remote, "Show all the repo remotes")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true)
      ->group("GIT OUTPUT OPTIONS");

    subcmd->usage("fima [DIRECTORY PATH] info [FILE PATH] [OPTIONS]");

    subcmd->callback([&]() { _info(options, path, repo); });
}

} // namespace commands

} // namespace fima
