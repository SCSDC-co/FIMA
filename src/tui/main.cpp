#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <stdio.h>
#include <string>
#include <vector>

#include "../../include/tui/main.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

void printTui() {
    using namespace ftxui;

    // temporary values
    std::string path = "really/cool/path";
    std::vector<std::string> dirDirs;
    std::vector<std::string> dirFiles;

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
