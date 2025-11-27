#include <filesystem>
#include <iostream>
#include <string>

// I ABSOLUTELY LOVE THIS LIBRARY
#include "../include/cli/CLI11.hpp"
#include "../include/commands/create/directory.h"
#include "../include/commands/create/file.h"
#include "../include/commands/ls/main.h"
#include "../include/commands/tree/main.h"
#include "../include/tui/main.h"

#define VERSION "1.0.0"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    CLI::App app{"FIMA - Fast, Minimal & Awesome File Manager"};
    argv = app.ensure_utf8(argv);

    bool tui{false};
    bool display_version{false};
    bool file_flag{false};
    bool dir_flag{false};
    std::string path_to_create;

    app.add_flag("-v,--version", display_version, "Shows the program version");

    fs::path path{fs::current_path()};
    app.add_option("directory", path,
                   "Directory to work on (default current directory)")
        ->check(CLI::ExistingDirectory)
        ->expected(0, 1);

    CLI::App *ls_subcmd = app.add_subcommand(
        "ls", "Prints the content of the directory like the ls command");

    ls_subcmd->add_flag("-n,--not-tui", tui, "Disable TUI")
        ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    CLI::App *tree_subcmd = app.add_subcommand(
        "tree", "Prints the tree of the directory like the tree command");

    tree_subcmd->add_flag("-n,--not-tui", tui, "Disable TUI")
        ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    CLI::App *create_subcmd =
        app.add_subcommand("create", "Create a dir or a file");

    create_subcmd->add_option("path", path_to_create, "Path to create");
    create_subcmd->add_flag("--file", file_flag, "Create a file");
    create_subcmd->add_flag("--dir", dir_flag, "Create a directory");

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

    if (*create_subcmd) {
        if (file_flag) {
            create_file(path);
        } else if (dir_flag) {
            create_dir(path);
        } else {
            std::cout << "You need to choose a directory or a file!" << '\n';
        }

        return 0;
    }

    start_tui(path);

    return 0;
}
