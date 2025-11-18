#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

#include "../../include/helpers/get_directories_entries.h"
#include "../../include/tui/main.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fs = std::filesystem;

void start_tui(fs::path path) {
    using namespace ftxui;

    std::vector<fs::path> listOfTheDirectory = get_directories_entries(path);

    std::vector<Element> dirDirs;

    std::vector<Element> dirFiles;

    for (const fs::path &entry : listOfTheDirectory) {
        auto name = entry.filename().string();

        if (fs::is_directory(entry)) {
            name += "/";

            dirDirs.push_back(text(name));
        } else {
            dirFiles.push_back(text(name));
        }
    }

    auto document = vbox({

        hflow({
            text(" PATH ") | border | color(Color::Green),
            text(path) | border | flex | color(Color::Green),
        }),

        border(

            vbox({
                vbox(dirDirs) | color(Color::Green),
                vbox(dirFiles) | color(Color::White),
            })) |
            flex | color(Color::Green),

        // hflow({
        //     text(" PATH ") | border | color(Color::Green),
        //     text(path) | border | flex | color(Color::Green),
        // }),

    });

    auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
    Render(screen, document);
    screen.Print();
}
