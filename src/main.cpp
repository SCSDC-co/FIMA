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
#include <ostream>
#include <string>
#include <vector>

// I ABSOLUTELY LOVE THIS LIBRARY
#include "cli/CLI11.hpp"
#include "commands/cloc/cloc.h"
#include "commands/copy/directory.h"
#include "commands/copy/file.h"
#include "commands/create/directory.h"
#include "commands/create/file.h"
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

    bool display_version{ false };
    bool reset_program_files{ false };
    bool preserve_config_file{ false };

    // names should be descriptive
    std::vector<_fs::path> path_to_remove;
    std::vector<_fs::path> perms_path;
    _fs::path path_to_copy;
    _fs::path destination;
    _fs::path old_name;
    _fs::path new_name;
    _fs::directory_entry path{ _fs::current_path() };

    // for getting the correct .git directory we must pass the full path
    fima::git::GitRepo repo = fima::git::GitRepo(std::filesystem::absolute(path));

    app.add_flag("-v,--version", display_version, "Show the program version")->configurable(false);

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

    /*  ==============
     *  LS SUB COMMAND
     */

    fima::options::ls_options ls_options;

    CLI::App* ls_subcmd =
      app.add_subcommand("ls", "Print the content of the directory like the ls command")
        ->configurable(false);

    ls_subcmd->add_flag("-i,--icons", ls_options.icons, "Put an icon next to the name of the item")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    ls_subcmd->add_flag("-a,--all", ls_options.all, "Will also count the dotfiles")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    ls_subcmd->add_flag("-l,--long", ls_options.long_output, "Display the file metadata")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    ls_subcmd
      ->add_flag(
        "-G,--no-gitignore", [&](int) { ls_options.gitignore = false; }, "Ignore .gitignore")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    ls_subcmd
      ->add_flag("-v,--verbose",
                 ls_options.verbose,
                 "Display the number of directories and files (only works with long output)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    ls_subcmd->usage("fima ls [OPTIONS]");

    ls_subcmd->callback([&]() { fima::commands::ls(path, repo, ls_options); });

    /*  ================
     *  TREE SUB COMMAND
     */

    fima::options::tree_options tree_options;

    CLI::App* tree_subcmd =
      app.add_subcommand("tree", "Print the tree of the directory like the tree command")
        ->configurable(false);

    tree_subcmd->add_flag("-a,--all", tree_options.all, "Show dotfiles")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    tree_subcmd
      ->add_flag(
        "-G,--no-gitignore", [&](int) { tree_options.gitignore = false; }, "Ignore .gitignore")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    tree_subcmd->add_flag("-v,--verbose", tree_options.verbose, "Verbose output")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    tree_subcmd->usage("fima tree [OPTIONS]");

    // when calling through the CLI it shouldn't be use a TUI
    tree_options.tui = false;

    tree_subcmd->callback([&]() { fima::commands::tree(path, repo, tree_options); });

    /*  ==================
     *  CREATE SUB COMMAND
     */

    CLI::App* create_subcmd =
      app.add_subcommand("create", "Create a directory or a file")->configurable(false);

    std::vector<_fs::path> create_file_paths;
    create_subcmd->add_option("-f,--file", create_file_paths, "Files to create")
      ->configurable(false);

    std::vector<_fs::path> create_dir_paths;
    create_subcmd->add_option("-d,--dir", create_dir_paths, "Directories to create")
      ->configurable(false);

    create_subcmd->usage("fima create -d [DIRECTORIES] -f [FILES]");

    create_subcmd->callback([&]() {
        fima::commands::create::dir(create_dir_paths);
        fima::commands::create::file(create_file_paths);
    });

    /*  ==================
     *  REMOVE SUB COMMAND
     */

    CLI::App* remove_subcmd =
      app.add_subcommand("remove", "Remove files and direcories (supports regex)")
        ->configurable(false);

    remove_subcmd->add_option("path", path_to_remove, "File or directory to remove")
      ->configurable(false)
      ->required(true);

    bool remove_subcmd_recursive{ false };
    remove_subcmd
      ->add_flag("-r,--recursive",
                 remove_subcmd_recursive,
                 "Remove directories and their contents recursively")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    remove_subcmd->usage("fima remove [PATHS] [OPTIONS]");

    remove_subcmd->callback([&]() {
        std::vector<std::regex> regexes;

        for (const _fs::path& path : path_to_remove) {
            regexes.push_back(fima::helpers::regex::glob_to_regex(path.filename().string()));
        }

        fima::commands::remove(regexes, remove_subcmd_recursive);
    });

    /*  ================
     *  COPY SUB COMMAND
     */

    CLI::App* copy_subcmd =
      app.add_subcommand("copy", "Copy a file or a directory")->configurable(false);

    copy_subcmd->add_option("source-file", path_to_copy, "File or directory copy")
      ->configurable(false)
      ->required(true);
    copy_subcmd->add_option("destination", destination, "Destination")
      ->configurable(false)
      ->required(true);

    copy_subcmd->usage("fima copy [SOURCE] [DESTINATION]");

    copy_subcmd->callback([&]() {
        if (_fs::is_regular_file(path_to_copy)) {
            fima::commands::copy::file(path_to_copy, destination);
        } else if (_fs::is_directory(path_to_copy)) {
            fima::commands::copy::directory(path_to_copy, destination);
        }
    });

    /*  ==================
     *  RENAME SUB COMMAND
     */

    CLI::App* rename_subcmd =
      app.add_subcommand("rename", "Rename/move a file or a directory")->configurable(false);

    rename_subcmd->add_option("old-name", old_name, "File or directory to move or rename")
      ->configurable(false)
      ->required(true);
    rename_subcmd->add_option("new-name", new_name, "The new name for the directory or file")
      ->configurable(false)
      ->required(true);

    rename_subcmd->usage("fima rename [OLD NAME] [NEW NAME]");

    rename_subcmd->callback([&]() { fima::commands::rename(old_name, new_name); });

    /*  =================
     *  PERMS SUB COMMAND
     */

    CLI::App* perms_subcmd =
      app.add_subcommand("perms", "Show a directory/file permissions")->configurable(false);

    perms_subcmd->add_option("path", perms_path, "The file path to read permissions from")
      ->configurable(false)
      ->required(true);

    perms_subcmd->usage("fima perms [PATHS]");

    perms_subcmd->callback([&]() { fima::commands::permissions(perms_path); });

    /*  ================
     *  CLOC SUB COMMAND
     */

    fima::options::cloc_options cloc_options;

    CLI::App* cloc_subcmd = app.add_subcommand("cloc", "Count lines of code")->configurable(false);

    cloc_subcmd
      ->add_option("paths", cloc_options.paths, "The paths to work on (default current directory) ")
      ->configurable(false);

    std::vector<_fs::path> cloc_ignore{};
    cloc_subcmd->add_option("--ignore,-i", cloc_ignore, "Paths to ignore")
      ->configurable(true)
      ->expected(0, -1);

    cloc_subcmd->add_option("--sort,-S", cloc_options.sorting, "Type of sorting")
      ->configurable(true)
      ->expected(0, 1);

    cloc_subcmd->add_flag("--quiet,-q", cloc_options.quiet, "Enables quiet output")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    cloc_subcmd
      ->add_flag(
        "--no-gitignore,-G", [&](int) { cloc_options.gitignore = false; }, "Ignore .gitignore")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    cloc_subcmd
      ->add_flag("--show-languages,-s",
                 cloc_options.show_languages,
                 "Shows all the languages that cloc supports")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(false);

    cloc_subcmd->usage("fima cloc [PATHS] [OPTIONS]");

    cloc_subcmd->callback([&]() {
        std::vector<std::regex> regexes;

        for (const _fs::path& path : cloc_ignore) {
            regexes.push_back(fima::helpers::regex::glob_to_regex(path.filename().string()));
        }

        fima::commands::cloc(regexes, repo, cloc_options);
    });

    /*  ================
     *  INFO SUB COMMAND
     */

    fima::options::info_options info_options;

    CLI::App* info_subcmd =
      app.add_subcommand("info", "Display information about a file or directory")
        ->configurable(false);

    info_subcmd
      ->add_option(
        "path", info_options.path, "The path to get the info from (default current directory)")
      ->configurable(false);

    info_subcmd
      ->add_flag("-g,--git", info_options.git, "Display git information instead of file/directory")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(false);

    info_subcmd->add_flag("-t,--tags", info_options.tags, "Show tags (only works with -g,--git)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    info_subcmd
      ->add_flag(
        "-r,--remotes", info_options.remote, "Show the remotes (only works with -g, --git)")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    info_subcmd->usage("fima info [PATH] [OPTIONS]");

    info_subcmd->callback([&]() { fima::commands::info(info_options, repo); });

    /*  ===============
     *  ZIP SUB COMMAND
     */

    std::vector<_fs::path> zip_paths{};
    _fs::path zip_output_path{};

    CLI::App* zip_subcmd =
      app.add_subcommand("zip", "Zip files into a .zip archive")->configurable(false);

    zip_subcmd->add_option("paths", zip_paths, "File to zip")->required(true)->configurable(false);
    zip_subcmd->add_option("-o,--output", zip_output_path, "The output archive")
      ->required(true)
      ->configurable(false);

    zip_subcmd->usage("fima zip [PATHS] -o [OUTPUT]");

    zip_subcmd->callback([&]() { fima::commands::zip(zip_paths, zip_output_path); });

    /*  =================
     *  UNZIP SUB COMMAND
     */

    _fs::path unzip_archive{};
    _fs::path unzip_output_path{};

    CLI::App* unzip_subcmd =
      app.add_subcommand("unzip", "Unzip .zip archives")->configurable(false);

    unzip_subcmd->add_option("archive", unzip_archive, "The archive to unzip")
      ->required(true)
      ->configurable(false);
    unzip_subcmd->add_option("-o,--output", unzip_output_path, "The output of the unzipped archive")
      ->required(true)
      ->configurable(false);

    unzip_subcmd->usage("fima unzip [ARCHIVE] -o [OUTPUT]");

    unzip_subcmd->callback([&]() { fima::commands::unzip(unzip_archive, unzip_output_path); });

    CLI11_PARSE(app, argc, argv);

    if (display_version) {
        std::cout << fima::config::VERSION << std::endl;

        return 0;
    } else if (reset_program_files) {
        fima::program_files::reset_config_files(preserve_config_file);

        return 0;
    } else if (app.get_subcommands().empty()) {
        fima::tui::start_tui(path);

        return 0;
    }

    return 0;
}
