/*
 * src/main.cpp
 *
 * The main file of FIMA, it defines the CLI and calls all subprograms
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

// I ABSOLUTELY LOVE THIS LIBRARY
#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <rang.hpp>
#include <string>
#include <vector>

#include "commands/cloc/cloc.h"
#include "commands/copy.h"
#include "commands/create.h"
#include "commands/info/info.h"
#include "commands/ls/ls.h"
#include "commands/permissions.h"
#include "commands/remove.h"
#include "commands/rename.h"
#include "commands/trash/trash.h"
#include "commands/tree.h"
#include "commands/unzip.h"
#include "commands/zip.h"
#include "config.h"
#include "fs/trash.h"
#include "git/GitRepo.h"
#include "options.h"
#include "theme.h"
#include "tui/tui.h"
#include "utility/regex.h"

namespace _fs = std::filesystem;

int
main(int argc, char** argv)
{
    fima::config::setup_variables();
    fima::config::parse_config_file();

    fima::theme::parse_theme_file();

    fima::fs::trash::setup_variables();

    CLI::App app;
    argv = app.ensure_utf8(argv);

    app.name("fima");
    app.description("Fast, Incredible, Minimal & Awesome File Manager");
    app.usage("fima [PATH] [OPTIONS] [SUBCOMMANDS]");
    app.footer("\nMade with love by SCSDC 󰋑 ");

    app.get_formatter()->column_width(25);
    app.get_formatter()->long_option_alignment_ratio(0.25);
    app.get_formatter()->label("POSITIONALS", "PATH");

    CLI::Option* config = app.set_config("-c,--config",
                                         fima::config::CONFIG_FILE_PATH,
                                         "Specify the config file (TOML format)",
                                         false);

    app
      .add_flag_callback(
        "--no-config",
        [config]() {
            config->clear();
            config->add_result(""); // an empty path disables config processing
        },
        "Ignore all configuration files")
      ->callback_priority(CLI::CallbackPriority::First);

    app.allow_config_extras(CLI::config_extras_mode::ignore);

    app.require_subcommand(0, 1);

    _fs::directory_entry path{ _fs::current_path() };

    // for getting the correct .git directory we must pass the full path
    fima::git::GitRepo repo = fima::git::GitRepo(std::filesystem::absolute(path));

    app.set_version_flag("-v,--version", std::string(fima::config::VERSION))->configurable(false);

    app.add_option("directory", path, "Directory to work on (default current directory)")
      ->check(CLI::ExistingDirectory)
      ->expected(0, 1)
      ->configurable(false);

    CLI::App* version_subcmd =
      app.add_subcommand("version", "Print the current version with some extra information")
        ->configurable(false);

    app.add_flag_callback("--dump-default-theme", [&]() {
        std::cout << "[general]" << '\n';
        std::cout << "primary = \"" << fima::theme::default_theme.primary.name_string << "\""
                  << '\n';
        std::cout << "secondary = \"" << fima::theme::default_theme.secondary.name_string << "\""
                  << '\n';
        std::cout << "border = \"" << fima::theme::default_theme.border.name_string << "\"" << '\n';
        std::cout << "info = \"" << fima::theme::default_theme.info.name_string << "\"" << '\n';
        std::cout << "warning = \"" << fima::theme::default_theme.warning.name_string << "\""
                  << '\n';
        std::cout << "error = \"" << fima::theme::default_theme.error.name_string << "\"" << '\n'
                  << '\n';

        std::cout << "[specific]" << '\n';
        std::cout << "directory = \"" << fima::theme::default_theme.directory.name_string << "\""
                  << '\n';
        std::cout << "executable = \"" << fima::theme::default_theme.executable.name_string << "\""
                  << '\n';
        std::cout << "symlink = \"" << fima::theme::default_theme.symlink.name_string << "\""
                  << '\n';
        std::cout << "archive = \"" << fima::theme::default_theme.archive.name_string << "\""
                  << '\n';
        std::cout << "media = \"" << fima::theme::default_theme.media.name_string << "\"" << '\n';
        std::cout << "normal_file = \"" << fima::theme::default_theme.normal_file.name_string
                  << "\"" << '\n'
                  << '\n';

        std::cout << "[permissions]" << '\n';
        std::cout << "read = \"" << fima::theme::default_theme.perms_read.name_string << "\""
                  << '\n';
        std::cout << "write = \"" << fima::theme::default_theme.perms_write.name_string << "\""
                  << '\n';
        std::cout << "execute = \"" << fima::theme::default_theme.perms_exec.name_string << "\""
                  << '\n';
        std::cout << "null = \"" << fima::theme::default_theme.perms_null.name_string << "\""
                  << '\n'
                  << '\n';

        std::cout << "[ls]" << '\n';
        std::cout << "permissions = \"" << fima::theme::default_theme.ls_permissions.name_string
                  << "\"" << '\n';
        std::cout << "size = \"" << fima::theme::default_theme.ls_size.name_string << "\"" << '\n';
        std::cout << "user = \"" << fima::theme::default_theme.ls_user.name_string << "\"" << '\n';
        std::cout << "date_modified = \"" << fima::theme::default_theme.ls_date_modified.name_string
                  << "\"" << '\n';
        std::cout << "name = \"" << fima::theme::default_theme.ls_name.name_string << "\"" << '\n';

        std::exit(0);
    });

    app.add_flag_callback("--dump-default-config", [&]() {
        std::cout << "depth = 8" << '\n';
        std::cout << "process_directory_size = false" << '\n' << '\n';

        std::cout << "[ls]" << '\n';
        std::cout << "icons = false" << '\n';
        std::cout << "all = false" << '\n';
        std::cout << "one-line = false" << '\n';
        std::cout << "group-directories-first = false" << '\n';
        std::cout << "group-directories-last = false" << '\n';
        std::cout << "long = false" << '\n';
        std::cout << "verbose = false" << '\n';
        std::cout << "headers = false" << '\n' << '\n';

        std::cout << "[tree]" << '\n';
        std::cout << "all = false" << '\n';
        std::cout << "no-gitignore = false" << '\n';
        std::cout << "verbose = false" << '\n' << '\n';

        std::cout << "[rm]" << '\n';
        std::cout << "recursive = false" << '\n' << '\n';

        std::cout << "[cloc]" << '\n';
        std::cout << "ignore = [ \"\" ]" << '\n';
        std::cout << "sort = \"total\"" << '\n';
        std::cout << "quiet = false" << '\n';
        std::cout << "no-gitignore = false" << '\n' << '\n';

        std::cout << "[info]" << '\n';
        std::cout << "verbose = false" << '\n';
        std::cout << "tags = false" << '\n';
        std::cout << "remotes = false" << '\n';

        std::exit(0);
    });

    version_subcmd->callback([&]() {
        std::cout << fima::theme::theme.primary;

        std::cout << fima::config::LOGO << '\n';

        std::cout << "Fast, Incredible, Minimal and Awesome File Manager" << '\n' << '\n';
        std::cout << "Version: " << fima::theme::theme.secondary << fima::config::VERSION << '\n';

        std::cout << fima::theme::Color::reset << '\n';
    });

    // ──────────────────────────────────────────────────────────────────────

    fima::options::ls_options ls_options{};

    fima::commands::setup_ls(app, ls_options);

    // ──────────────────────────────────────────────────────────────────────

    fima::options::tree_options tree_options{};

    fima::commands::setup_tree(app, path, repo, tree_options);

    // ──────────────────────────────────────────────────────────────────────

    std::vector<_fs::path> create_file_paths{};
    std::vector<_fs::path> create_dir_paths{};

    fima::commands::setup_create(app, create_file_paths, create_dir_paths);

    // ──────────────────────────────────────────────────────────────────────

    fima::options::rm_options rm_options{};

    fima::commands::setup_remove(app, rm_options);

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

    fima::commands::setup_info(app, path, repo, info_options);

    // ──────────────────────────────────────────────────────────────────────

    std::vector<std::filesystem::path> zip_paths{};
    std::filesystem::path zip_output_path{};

    fima::commands::setup_zip(app, zip_paths, zip_output_path);

    // ──────────────────────────────────────────────────────────────────────

    std::filesystem::path unzip_archive{};
    std::filesystem::path unzip_output_path{};

    fima::commands::setup_unzip(app, unzip_archive, unzip_output_path);

    // ──────────────────────────────────────────────────────────────────────

    bool trash_yes{ false };

    fima::commands::setup_trash(app, trash_yes);

    // ──────────────────────────────────────────────────────────────────────

    CLI11_PARSE(app, argc, argv);

    if (app.get_subcommands().empty()) {
        fima::tui::start_tui(path);

        return 0;
    }

    return 0;
}
