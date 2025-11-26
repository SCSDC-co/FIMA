#include <filesystem>
#include <iostream>
#include <string>

// I ABSOLUTELY LOVE THIS LIBRARY
#include "../include/cli/CLI11.hpp"
#include "../include/commands/ls/main.h"
#include "../include/commands/tree/main.h"
#include "../include/tui/main.h"

#define VERSION "1.0.0"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    CLI::App app{"FIMA - Fast, Minimal & Awesome File Manager"};
    argv = app.ensure_utf8(argv);

    bool display_version{false};
    app.add_flag("-v,--version", display_version, "Shows the program version");

    fs::path path{fs::current_path()};
    app.add_option("directory", path,
                   "Directory to work on (default current directory)")
        ->check(CLI::ExistingDirectory)
        ->expected(0, 1);

    bool tui{false};
    app.add_flag("-n,--not-tui", tui, "Disable TUI (only works on ls and tree)")
        ->multi_option_policy(CLI::MultiOptionPolicy::Throw);

    CLI::App *ls_subcmd = app.add_subcommand(
        "ls", "Prints the content of the directory like the ls command");

    CLI::App *tree_subcmd = app.add_subcommand(
        "tree", "Prints the tree of the directory like the tree command");

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

    start_tui(path);

    return 0;
}
