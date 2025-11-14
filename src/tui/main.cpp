#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <stdio.h>
#include <string>
#include <vector>

#include "../../include/helpers/list_directories.h"
#include "../../include/tui/main.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

void printTui(std::string path) {
    using namespace ftxui;

    std::vector<std::string> listOfTheDirectory = list_directories(path);

    std::vector<Element> dirDirs;

    std::vector<Element> dirFiles;

    for (std::string dir : listOfTheDirectory) {
        int lastSlash = dir.rfind('/');

        dir = dir.erase(0, lastSlash + 1);

        if (std::filesystem::is_directory(dir)) {
            dir.append("/");

            dirDirs.push_back(text(dir));
        } else {
            dirFiles.push_back(text(dir));
        }
    }

    auto document = vbox({

        hflow({
            text(" PATH ") | border | color(Color::Green),
            text(path) | border | flex | color(Color::Green),
        }),

        border(

            vbox({
                vbox(dirDirs) | color(Color::White),
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
    getchar();
}
