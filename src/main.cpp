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
#include "config.h"
#include "git/GitRepo.h"
#include "options.h"
#include "program_files.h"
#include "tui/tui.h"
#include "utility/colors.h"
#include "utility/regex.h"

namespace fs = std::filesystem;

int
main(int argc, char** argv)
{
    fima::program_files::setup_variables();

    CLI::App app;
    argv = app.ensure_utf8(argv);

    app.name("fima");
    app.description("FIMA - Fast, Incredible, Minimal & Awesome File Manager");

    app.get_formatter()->column_width(25);
    app.get_formatter()->long_option_alignment_ratio(0.3);

    app
      .set_config(
        "--config", fima::program_files::CONFIG_FILE_PATH, "Specify the config file (TOML format)")
      ->transform(CLI::FileOnDefaultPath(fima::program_files::CONFIG_FILE_PATH))
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    bool tui{ false };
    bool display_version{ false };
    bool reset_program_files{ false };
    bool preserve_config_file{ false };

    // names should be descriptive
    std::vector<fs::path> path_to_create_or_remove;
    std::vector<fs::path> perms_path;
    fs::path path_to_copy;
    fs::path destination;
    fs::path old_name;
    fs::path new_name;

    app.add_flag("-v,--version", display_version, "Shows the program version")->configurable(false);
    CLI::App* version_subcmd =
      app.add_subcommand("version", "Print the current version with some extra information")
        ->configurable(false);

    app.add_flag("--reset-config-files", reset_program_files, "Resets the config files")
      ->configurable(false);

    app
      .add_flag("--preserve-config-file",
                preserve_config_file,
                "Preserces the config.toml file (only work when using --reset-config-files)")
      ->configurable(true);

    fs::directory_entry path{ fs::current_path() };
    app.add_option("directory", path, "Directory to work on (default current directory)")
      ->check(CLI::ExistingDirectory)
      ->expected(0, 1)
      ->configurable(false);

    /*  ==============
     *  LS SUB COMMAND
     */

    fima::options::ls_options ls_options;

    CLI::App* ls_subcmd =
      app.add_subcommand("ls", "Prints the content of the directory like the ls command")
        ->configurable(false);

    ls_subcmd->add_flag("-i,--icons", ls_options.icons, "Puts an icon next to the name of the item")
      ->configurable(true);

    ls_subcmd->add_flag("-a,--all", ls_options.all, "Will also count the dotfiles")
      ->configurable(true);

    ls_subcmd->add_flag("-l,--long", ls_options.long_output, "Display the file metadata")
      ->configurable(true);

    ls_subcmd
      ->add_flag("-v,--verbose",
                 ls_options.verbose,
                 "Displays the number of directories and files (only works with long output)")
      ->configurable(true);

    /*  ================
     *  TREE SUB COMMAND
     */

    CLI::App* tree_subcmd =
      app.add_subcommand("tree", "Prints the tree of the directory like the tree command")
        ->configurable(false);

    tree_subcmd->add_flag("-n,--no-tui", tui, "Disable TUI")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    /*  ==================
     *  CREATE SUB COMMAND
     */

    CLI::App* create_subcmd =
      app.add_subcommand("create", "Create a directory or a file")->configurable(false);

    create_subcmd->add_option("path", path_to_create_or_remove, "File or dir to create")
      ->configurable(false);
    create_subcmd->add_subcommand("dir", "Creates a directory")->configurable(false);
    create_subcmd->add_subcommand("file", "Creates a file")->configurable(false);

    /*  ==================
     *  REMOVE SUB COMMAND
     */

    CLI::App* remove_subcmd =
      app.add_subcommand("remove", "Removes a file or a directory")->configurable(false);

    remove_subcmd->add_option("path", path_to_create_or_remove, "File or directory to remove")
      ->configurable(false)
      ->required(true);

    bool remove_subcmd_recursive{ false };
    remove_subcmd
      ->add_flag("-r,--recursive",
                 remove_subcmd_recursive,
                 "Remove directories and their contents recursively")
      ->configurable(true);

    /*  ================
     *  COPY SUB COMMAND
     */

    CLI::App* copy_subcmd =
      app.add_subcommand("copy", "Copy a file or a directory")->configurable(false);

    copy_subcmd->add_option("source-file", path_to_copy, "File or directory copy")
      ->configurable(false);
    copy_subcmd->add_option("destination", destination, "Destination")->configurable(false);

    /*  ==================
     *  RENAME SUB COMMAND
     */

    CLI::App* rename_subcmd =
      app.add_subcommand("rename", "Rename/move a file or a directory")->configurable(false);

    rename_subcmd->add_option("old-name", old_name, "File or directory to move or rename")
      ->configurable(false);
    rename_subcmd->add_option("new-name", new_name, "The new name for the directory or file")
      ->configurable(false);

    /*  =================
     *  PERMS SUB COMMAND
     */

    CLI::App* perms_subcmd =
      app.add_subcommand("perms", "Shows a directory/file permissions")->configurable(false);

    perms_subcmd->add_option("path", perms_path, "The file path to read permissions from")
      ->configurable(false);

    /*  ================
     *  CLOC SUB COMMAND
     */

    fima::options::cloc_options cloc_options;

    CLI::App* cloc_subcmd =
      app.add_subcommand("cloc", "Count lines of code of a file")->configurable(false);

    cloc_subcmd
      ->add_option("paths", cloc_options.paths, "The paths to work on (default current directory)")
      ->configurable(false);

    std::vector<fs::path> cloc_ignore{};
    cloc_subcmd->add_option("--ignore,-i", cloc_ignore, "Paths to ignore")
      ->configurable(true)
      ->expected(0, -1);

    cloc_subcmd->add_option("--sort,-S", cloc_options.sorting, "Type of sorting")
      ->configurable(true);

    cloc_subcmd->add_flag("--quiet,-q", cloc_options.quiet, "Enables quiet output")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    cloc_subcmd
      ->add_flag("--show-languages,-s",
                 cloc_options.show_languages,
                 "Shows all the languages that cloc supports")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(false);

    /*  ================
     *  INFO SUB COMMAND
     */

    fima::options::info_options info_options;

    CLI::App* info_subcmd =
      app.add_subcommand("info", "Displays informations about a file or directory")
        ->configurable(false);

    info_subcmd
      ->add_option(
        "path", info_options.path, "The path to get the info from (default current directory)")
      ->configurable(false);

    info_subcmd
      ->add_flag("-g,--git", info_options.git, "Displays git information instead of file/directory")
      ->configurable(false);

    CLI11_PARSE(app, argc, argv);

    // since in CLI11 we can't do true -> false we need to do false -> true and then negate it to
    // get the correct value
    tui = !tui;

    // for getting the correct .git directory we must pass the full path
    fima::git::GitRepo repo = fima::git::GitRepo(std::filesystem::absolute(path));

    if (display_version) {
        std::cout << fima::config::VERSION << std::endl;

        return 0;
    } else if (reset_program_files) {
        fima::program_files::reset_config_files(preserve_config_file);

        return 0;
    } else if (app.got_subcommand(version_subcmd)) {
        std::cout << fima::colors::GREEN;

        std::cout << fima::config::LOGO << '\n';

        std::cout << "Fast, Incredible, Minimal and Awesome File Manager" << '\n' << '\n';
        std::cout << "Version: " << fima::colors::RESET << fima::config::VERSION << '\n';

        std::cout << '\n';

        return 0;
    } else if (app.got_subcommand(ls_subcmd)) {
        fima::ls::start(path, ls_options);

        return 0;
    } else if (app.got_subcommand(tree_subcmd)) {
        fima::tree::start(path, "", tui);

        return 0;
    } else if (create_subcmd->got_subcommand("dir")) {
        fima::create::dir(path_to_create_or_remove);

        return 0;
    } else if (create_subcmd->got_subcommand("file")) {
        fima::create::file(path_to_create_or_remove);

        return 0;
    } else if (app.got_subcommand(remove_subcmd)) {
        std::vector<std::regex> regexes;

        for (const fs::path& path : path_to_create_or_remove) {
            regexes.push_back(fima::helpers::regex::glob_to_regex(path.filename().string()));
        }

        fima::remove(regexes, remove_subcmd_recursive);

        return 0;
    } else if (app.got_subcommand(copy_subcmd)) {
        if (fs::is_regular_file(path_to_copy)) {
            fima::copy::file(path_to_copy, destination);
        } else if (fs::is_directory(path_to_copy)) {
            fima::copy::directory(path_to_copy, destination);
        }

        return 0;
    } else if (app.got_subcommand(rename_subcmd)) {
        fima::rename(old_name, new_name);

        return 0;
    } else if (app.got_subcommand(perms_subcmd)) {
        fima::perms::permissions(perms_path);

        return 0;
    } else if (app.got_subcommand(cloc_subcmd)) {
        std::vector<std::regex> regexes;

        for (const fs::path& path : cloc_ignore) {
            regexes.push_back(fima::helpers::regex::glob_to_regex(path.filename().string()));
        }

        fima::cloc::main(regexes, cloc_options);

        return 0;
    } else if (app.got_subcommand(info_subcmd)) {
        fima::info::info(info_options, repo);

        return 0;
    }

    fima::tui::start_tui(path);

    return 0;
}
