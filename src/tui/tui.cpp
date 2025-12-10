#include "../../include/tui/tui.h"

#include <algorithm>
#include <filesystem>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

#include "../../include/helpers/get_directories_entries.h"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

namespace fs = std::filesystem;

namespace fima {

namespace tui {

void
start_tui(fs::path path)
{
    using namespace ftxui;

    std::vector<fs::directory_entry> list_of_the_directory{
        fima::helpers::get_directories_entries(path)
    };

    sort(list_of_the_directory.begin(),
         list_of_the_directory.end(),
         [](auto& a, auto& b) {
             if (a.is_directory() && !b.is_directory()) {
                 return true;
             }

             if (!a.is_directory() && b.is_directory()) {
                 return false;
             }

             return a.path().filename() < b.path().filename();
         });

    std::vector<Element> path_entries;

    for (const fs::path& entry : list_of_the_directory) {
        auto name = entry.filename().string();

        if (fs::is_directory(entry)) {
            name += "/";
        }

        path_entries.push_back(text(name) |
                               (fs::is_directory(entry) ? color(Color::Green)
                                                        : color(Color::White)));
    }

    auto document = vbox({

      hflow({
        text(" PATH ") | border | color(Color::Green),
        text(path) | border | flex | color(Color::Green),
      }),

      border(

        vbox({ vbox(path_entries) })) |
        flex | color(Color::Green),
    });

    auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
    Render(screen, document);
    screen.Print();
}

} // namespace tui

} // namespace fima
