#include <filesystem>
#include <string>

// I ABSOLUTELY LOVE THIS LIBRARY
#include "../include/cli/CLI11.hpp"
#include "../include/modules/ls/main.h"
#include "../include/tui/main.h"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    CLI::App app{"FIMA - Fast, Minimal & Awesome File Manager"};
    argv = app.ensure_utf8(argv);

    fs::path path{fs::current_path()};
    app.add_option("directory", path,
                   "Directory to work on (default current directory)")
        ->check(CLI::ExistingDirectory)
        ->expected(0, 1);

    CLI::App *ls_subcmd = app.add_subcommand(
        "ls",
        "Prints the content of the current directory like the ls command");

    CLI11_PARSE(app, argc, argv);

    if (*ls_subcmd) {
        ls(path);

        return 0;
    }

    start_tui(path);

    return 0;
}
