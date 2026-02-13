#include "commands/cloc/helpers/print_table.h"

#include <array>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "commands/cloc/helpers/LanguageStats.h"
#include "ftxui/dom/node.hpp"

namespace fima {

namespace cloc {

namespace helpers {

void
print_table(std::unordered_map<std::string, fima::cloc::classes::LanguageStats> language_map)
{
    using namespace ftxui;

    std::vector<std::pair<std::string, fima::cloc::classes::LanguageStats>> sorted;

    for (const auto& it : language_map) {
        sorted.push_back(it);
    }

    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second.get_total() > b.second.get_total();
    });

    std::vector<std::vector<std::string>> table_data;

    std::vector<std::string> table_header = { "Language",   "Files",    "Total Lines",
                                              "Code Lines", "Comments", "Blank Lines" };

    table_data.push_back(table_header);

    for (const auto& [name, stats] : sorted) {
        table_data.push_back({ name,
                               std::to_string(stats.get_files()),
                               std::to_string(stats.get_total()),
                               std::to_string(stats.get_code()),
                               std::to_string(stats.get_comment()),
                               std::to_string(stats.get_blank()) });
    }

    Table table = Table(table_data);

    table.SelectAll().Border(ROUNDED);

    TableSelection first_row = table.SelectRow(0);
    first_row.Border(ROUNDED);
    first_row.SeparatorVertical();
    first_row.DecorateCells(color(Color::Green));

    table.SelectRows(1, -1).SeparatorHorizontal();

    table.SelectRectangle(1, -1, 1, -1).DecorateCells(align_right);

    Element document = table.Render();
    Screen screen    = ftxui::Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

}

}

}
