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

#include "commands/info/directory.h"
#include "commands/info/file.h"
#include "commands/info/git.h"
#include "logger.h"
#include "utility/colors.h"

void
_info(const fima::options::info_options& options, fima::git::GitRepo& repo)
{
    if (!options.path.exists()) {
        fima::logger::error(true,
                            "info",
                            fima::colors::RED + "The path doesn't exist: " + fima::colors::RESET +
                              "{}",
                            options.path.path().string());

        return;
    }

    if (options.git) {
        repo.change_repo_path(options.path);

        fima::info::git(options, repo);

        return;
    }

    if (options.path.is_directory()) {
        fima::info::dir(options.path);
    } else {
        fima::info::file(options.path);
    }
}

namespace fima {

namespace commands {

void
setup_info(CLI::App& app, fima::git::GitRepo& repo, fima::options::info_options& options)
{
    CLI::App* subcmd = app.add_subcommand("info", "Display information about a file or directory")
                         ->configurable(false);

    subcmd
      ->add_option(
        "path", options.path, "The path to get the info from (default current directory)")
      ->configurable(false);

    subcmd->add_flag("-g,--git", options.git, "Display git information instead of file/directory")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(false);

    subcmd->add_flag("-t,--tags", options.tags, "Show tags (only works with -g,--git)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->add_flag("-r,--remotes", options.remote, "Show the remotes (only works with -g, --git)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd->usage("fima info [PATH] [OPTIONS]");

    subcmd->callback([&]() { _info(options, repo); });
}

} // namespace commands

} // namespace fima
