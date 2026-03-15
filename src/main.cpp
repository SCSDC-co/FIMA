/*
 * src/main.cpp
 *
 * The main file of FIMA, it defines the CLI and calls all subprograms
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// I ABSOLUTELY LOVE THIS LIBRARY
#include "cli/CLI11.hpp"
#include "commands/cloc/cloc.h"
#include "commands/copy.h"
#include "commands/create.h"
#include "commands/info/info.h"
#include "commands/ls/ls.h"
#include "commands/permissions.h"
#include "commands/remove.h"
#include "commands/rename.h"
#include "commands/tree.h"
#include "commands/unzip.h"
#include "commands/zip.h"
#include "config.h"
#include "git/GitRepo.h"
#include "options.h"
#include "program_files.h"
#include "tui/tui.h"
#include "utility/colors.h"
#include "utility/regex.h"

namespace _fs = std::filesystem;

int
main(int argc, char** argv)
{
    fima::program_files::setup_variables();
    fima::config::parse_config_file();

    CLI::App app;
    argv = app.ensure_utf8(argv);

    app.name("fima");
    app.description("FIMA - Fast, Incredible, Minimal & Awesome File Manager");
    app.usage("fima [TARGET] [OPTIONS] [SUBCOMMANDS]");
    app.footer("\nMade with love by SCSDC 󰋑 ");

    app.get_formatter()->column_width(25);
    app.get_formatter()->long_option_alignment_ratio(0.25);
    app.get_formatter()->label("POSITIONALS", "TARGET");

    app
      .set_config(
        "--config", fima::program_files::CONFIG_FILE_PATH, "Specify the config file (TOML format) ")
      ->transform(CLI::FileOnDefaultPath(fima::program_files::CONFIG_FILE_PATH))
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw);
    app.allow_config_extras(CLI::config_extras_mode::ignore);

    app.require_subcommand(0, 1);

    bool reset_program_files{ false };
    bool preserve_config_file{ false };

    _fs::directory_entry path{ _fs::current_path() };

    // for getting the correct .git directory we must pass the full path
    fima::git::GitRepo repo = fima::git::GitRepo(std::filesystem::absolute(path));

    app.set_version_flag("-v,--version", std::string(fima::config::VERSION))->configurable(false);

    app.add_flag("--reset-config-files", reset_program_files, "Reset the config files")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(false);

    app
      .add_flag("--preserve-config-file",
                preserve_config_file,
                "Preserve the config.toml file (only work when using --reset-config-files)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    app.add_option("directory", path, "Directory to work on (default current directory)")
      ->check(CLI::ExistingDirectory)
      ->expected(0, 1)
      ->configurable(false);

    CLI::App* version_subcmd =
      app.add_subcommand("version", "Print the current version with some extra information")
        ->configurable(false);

    version_subcmd->callback([&]() {
        std::cout << fima::colors::GREEN;

        std::cout << fima::config::LOGO << '\n';

        std::cout << "Fast, Incredible, Minimal and Awesome File Manager" << '\n' << '\n';
        std::cout << "Version: " << fima::colors::RESET << fima::config::VERSION << '\n';

        std::cout << '\n';
    });

    // ──────────────────────────────────────────────────────────────────────

    fima::options::ls_options ls_options{};

    fima::commands::setup_ls(app, path, repo, ls_options);

    // ──────────────────────────────────────────────────────────────────────

    fima::options::tree_options tree_options{};

    fima::commands::setup_tree(app, path, repo, tree_options);

    // ──────────────────────────────────────────────────────────────────────

    std::vector<_fs::path> create_file_paths{};
    std::vector<_fs::path> create_dir_paths{};

    fima::commands::setup_create(app, create_file_paths, create_dir_paths);

    // ──────────────────────────────────────────────────────────────────────

    std::vector<_fs::path> remove_paths{};

    fima::commands::setup_remove(app, remove_paths);

    // ──────────────────────────────────────────────────────────────────────

    _fs::path copy_source{};
    _fs::path copy_destination{};

    fima::commands::setup_copy(app, copy_source, copy_destination);

    // ──────────────────────────────────────────────────────────────────────

    _fs::path rename_old_name{};
    _fs::path rename_new_name{};

    fima::commands::setup_rename(app, rename_old_name, rename_new_name);

    // ──────────────────────────────────────────────────────────────────────

    std::vector<_fs::path> permissions_paths{};

    fima::commands::setup_permissions(app, permissions_paths);

    // ──────────────────────────────────────────────────────────────────────

    fima::options::cloc_options cloc_options{};

    fima::commands::setup_cloc(app, repo, cloc_options);

    // ──────────────────────────────────────────────────────────────────────

    fima::options::info_options info_options{};

    fima::commands::setup_info(app, repo, info_options);

    // ──────────────────────────────────────────────────────────────────────

    std::vector<std::filesystem::path> zip_paths{};
    std::filesystem::path zip_output_path{};

    fima::commands::setup_zip(app, zip_paths, zip_output_path);

    // ──────────────────────────────────────────────────────────────────────

    std::filesystem::path unzip_archive{};
    std::filesystem::path unzip_output_path{};

    fima::commands::setup_unzip(app, unzip_archive, unzip_output_path);

    // ──────────────────────────────────────────────────────────────────────

    CLI11_PARSE(app, argc, argv);

    if (reset_program_files) {
        fima::program_files::reset_config_files(preserve_config_file);

        return 0;
    } else if (app.get_subcommands().empty()) {
        fima::tui::start_tui(path);

        return 0;
    }

    return 0;
}
