#include <filesystem>
#include <iostream>
#include <string>

// I ABSOLUTELY LOVE THIS LIBRARY
#include "../include/cli/CLI11.hpp"
#include "../include/commands/copy/directory.h"
#include "../include/commands/copy/file.h"
#include "../include/commands/create/directory.h"
#include "../include/commands/create/file.h"
#include "../include/commands/ls/main.h"
#include "../include/commands/remove/directory.h"
#include "../include/commands/remove/file.h"
#include "../include/commands/rename/main.h"
#include "../include/commands/tree/main.h"
#include "../include/tui/main.h"

#define VERSION "1.0.0"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    CLI::App app{"FIMA - Fast, Minimal & Awesome File Manager"};
    argv = app.ensure_utf8(argv);

    bool tui{false};
    bool display_version{false};

    // names should be descriptive
    fs::path path_to_create_or_remove;
    fs::path path_to_copy;
    fs::path destination;
    fs::path old_name;
    fs::path new_name;

    app.add_flag("-v,--version", display_version, "Shows the program version");

    fs::path path{fs::current_path()};
    app.add_option("directory", path,
                   "Directory to work on (default current directory)")
        ->check(CLI::ExistingDirectory)
        ->expected(0, 1);

    /*  ==============
     *  LS SUB COMMAND
     */

    CLI::App *ls_subcmd = app.add_subcommand(
        "ls", "Prints the content of the directory like the ls command");

    ls_subcmd->add_flag("-n,--not-tui", tui, "Disable TUI")
        ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    /*  ================
     *  TREE SUB COMMAND
     */

    CLI::App *tree_subcmd = app.add_subcommand(
        "tree", "Prints the tree of the directory like the tree command");

    tree_subcmd->add_flag("-n,--not-tui", tui, "Disable TUI")
        ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    /*  ==================
     *  CREATE SUB COMMAND
     */

    CLI::App *create_subcmd =
        app.add_subcommand("create", "Create a directory or a file");

    create_subcmd->add_option("path", path_to_create_or_remove,
                              "File or dir to create");
    create_subcmd->add_subcommand("dir", "Creates a directory");
    create_subcmd->add_subcommand("file", "Creates a file");

    /*  ==================
     *  REMOVE SUB COMMAND
     */

    CLI::App *remove_subcmd =
        app.add_subcommand("remove", "Removes a file or a directory");

    remove_subcmd->add_option("path", path_to_create_or_remove,
                              "File or directory to remove");

    /*  ================
     *  COPY SUB COMMAND
     */

    CLI::App *copy_subcmd =
        app.add_subcommand("copy", "Copy a file or a directory");

    copy_subcmd->add_option("source-file", path_to_copy,
                            "File or directory copy");
    copy_subcmd->add_option("destination", destination, "Destination");

    /*  ==================
     *  RENAME SUB COMMAND
     */

    CLI::App *rename_subcmd =
        app.add_subcommand("rename", "Rename/move a file or a directory");

    rename_subcmd->add_option("old-name", old_name,
                              "File or directory to move or rename");
    rename_subcmd->add_option("new-name", new_name,
                              "The new name for the directory or file");

    CLI11_PARSE(app, argc, argv);

    tui = !tui;

    if (display_version) {
        std::cout << "FIMA version " << VERSION << std::endl;

        return 0;
    }

    if (*ls_subcmd) {
        ls(path, tui);

        return 0;
    }

    if (*tree_subcmd) {
        tree(path, "", tui);

        return 0;
    }

    if (create_subcmd->got_subcommand("dir")) {
        create_dir(path_to_create_or_remove);

        return 0;
    } else if (create_subcmd->got_subcommand("file")) {
        create_file(path_to_create_or_remove);

        return 0;
    }

    if (*remove_subcmd) {
        if (fs::is_regular_file(path_to_create_or_remove)) {
            remove_file(path_to_create_or_remove);
        } else if (fs::is_directory(path_to_create_or_remove)) {
            remove_dir(path_to_create_or_remove);
        }

        return 0;
    }

    if (*copy_subcmd) {
        if (fs::is_regular_file(path_to_copy)) {
            fima_copy_file(path_to_copy, destination);
        } else if (fs::is_directory(path_to_copy)) {
            fima_copy_directory(path_to_copy, destination);
        }

        return 0;
    }

    if (*rename_subcmd) {
        fima_rename(old_name, new_name);

        return 0;
    }

    start_tui(path);

    return 0;
}
