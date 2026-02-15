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
#include <unordered_set>
#include <vector>

// I ABSOLUTELY LOVE THIS LIBRARY
#include "cli/CLI11.hpp"
#include "commands/cloc/cloc.h"
#include "commands/copy/directory.h"
#include "commands/copy/file.h"
#include "commands/create/directory.h"
#include "commands/create/file.h"
#include "commands/ls.h"
#include "commands/permissions.h"
#include "commands/remove.h"
#include "commands/rename.h"
#include "commands/tree.h"
#include "config.h"
#include "helpers/colors.h"
#include "tui/tui.h"

namespace fs = std::filesystem;

int
main(int argc, char** argv)
{
    CLI::App app{ "FIMA - Fast, Incredible, Minimal & Awesome File Manager" };
    argv = app.ensure_utf8(argv);

    bool tui{ false };
    bool display_version{ false };

    // names should be descriptive
    std::vector<fs::path> path_to_create_or_remove;
    std::vector<fs::path> perms_path;
    fs::path path_to_copy;
    fs::path destination;
    fs::path old_name;
    fs::path new_name;

    app.add_flag("-v,--version", display_version, "Shows the program version");
    CLI::App* version_subcmd =
      app.add_subcommand("version", "Print the current version with some extra information");

    fs::path path{ fs::current_path() };
    app.add_option("directory", path, "Directory to work on (default current directory)")
      ->check(CLI::ExistingDirectory)
      ->expected(0, 1);

    /*  ==============
     *  LS SUB COMMAND
     */

    CLI::App* ls_subcmd =
      app.add_subcommand("ls", "Prints the content of the directory like the ls command");

    ls_subcmd->add_flag("-n,--not-tui", tui, "Disable TUI")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    /*  ================
     *  TREE SUB COMMAND
     */

    CLI::App* tree_subcmd =
      app.add_subcommand("tree", "Prints the tree of the directory like the tree command");

    tree_subcmd->add_flag("-n,--not-tui", tui, "Disable TUI")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    /*  ==================
     *  CREATE SUB COMMAND
     */

    CLI::App* create_subcmd = app.add_subcommand("create", "Create a directory or a file");

    create_subcmd->add_option("path", path_to_create_or_remove, "File or dir to create");
    create_subcmd->add_subcommand("dir", "Creates a directory");
    create_subcmd->add_subcommand("file", "Creates a file");

    /*  ==================
     *  REMOVE SUB COMMAND
     */

    CLI::App* remove_subcmd = app.add_subcommand("remove", "Removes a file or a directory");

    remove_subcmd->add_option("path", path_to_create_or_remove, "File or directory to remove");

    /*  ================
     *  COPY SUB COMMAND
     */

    CLI::App* copy_subcmd = app.add_subcommand("copy", "Copy a file or a directory");

    copy_subcmd->add_option("source-file", path_to_copy, "File or directory copy");
    copy_subcmd->add_option("destination", destination, "Destination");

    /*  ==================
     *  RENAME SUB COMMAND
     */

    CLI::App* rename_subcmd = app.add_subcommand("rename", "Rename/move a file or a directory");

    rename_subcmd->add_option("old-name", old_name, "File or directory to move or rename");
    rename_subcmd->add_option("new-name", new_name, "The new name for the directory or file");

    /*  =================
     *  PERMS SUB COMMAND
     */

    CLI::App* perms_subcmd = app.add_subcommand("perms", "Shows a directory/file permissions");

    perms_subcmd->add_option("path", perms_path, "The file path to read permissions from");

    /*  =================
     *  CLOC SUB COMMAND
     */

    std::vector<fs::path> cloc_paths{ fs::current_path() };
    CLI::App* cloc_subcmd = app.add_subcommand("cloc", "Count lines of code of a file");

    std::unordered_set<fs::path> paths_to_ignore{};
    cloc_subcmd->add_option("--ignore,-i", paths_to_ignore, "Paths to ignore");
    cloc_subcmd->add_option(
      "paths", cloc_paths, "The paths to work on (default current directory)");

    bool cloc_show_languages{ false };
    cloc_subcmd
      ->add_flag(
        "--show-languages,-s", cloc_show_languages, "Shows all the languages that cloc supports")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    CLI11_PARSE(app, argc, argv);

    // since in CLI11 we can't do true -> false we need to do false -> true and then negate it to
    // get the correct value
    tui = !tui;

    if (display_version) {
        std::cout << fima::config::VERSION << std::endl;

        return 0;
    }

    if (*version_subcmd) {
        std::cout << fima::colors::GREEN;

        std::cout << fima::config::LOGO << '\n';

        std::cout << "Fast, Incredible, Minimal and Awesome File Manager" << '\n' << '\n';
        std::cout << "Version: " << fima::colors::RESET << fima::config::VERSION << '\n';

        std::cout << '\n';

        return 0;
    }

    if (*ls_subcmd) {
        fima::ls::start(path, tui);

        return 0;
    }

    if (*tree_subcmd) {
        fima::tree::start(path, "", tui);

        return 0;
    }

    if (create_subcmd->got_subcommand("dir")) {
        fima::create::dir(path_to_create_or_remove);

        return 0;
    } else if (create_subcmd->got_subcommand("file")) {
        fima::create::file(path_to_create_or_remove);

        return 0;
    }

    if (*remove_subcmd) {
        fima::remove(path_to_create_or_remove);

        return 0;
    }

    if (*copy_subcmd) {
        if (fs::is_regular_file(path_to_copy)) {
            fima::copy::file(path_to_copy, destination);
        } else if (fs::is_directory(path_to_copy)) {
            fima::copy::directory(path_to_copy, destination);
        }

        return 0;
    }

    if (*rename_subcmd) {
        fima::rename(old_name, new_name);

        return 0;
    }

    if (*perms_subcmd) {
        fima::get_perms(perms_path);

        return 0;
    }

    if (*cloc_subcmd) {
        fima::cloc::main(cloc_paths, cloc_show_languages, paths_to_ignore);

        return 0;
    }

    fima::tui::start_tui(path);

    return 0;
}
