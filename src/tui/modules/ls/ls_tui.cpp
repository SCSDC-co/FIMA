#include "../../../../include/tui/modules/ls/ls_tui.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

void ls_tui(std::vector<std::string> dirs_vector,
            std::vector<std::string> files_vector) {
    using namespace ftxui;

    std::vector<std::vector<Element>> rows;

    rows.push_back({text("DIRS"), text("FILES")});

    size_t max_rows{std::max(dirs_vector.size(), files_vector.size())};

    for (size_t i = 0; i < max_rows; ++i) {
        Element dir_cell{i < dirs_vector.size() ? text(dirs_vector[i])
                                                : text("")};

        Element file_cell{i < files_vector.size() ? text(files_vector[i])
                                                  : text("")};

        rows.push_back({dir_cell, file_cell});
    }

    auto table = Table(rows);

    table.SelectAll().Border(ROUNDED);
    table.SelectAll().SeparatorVertical(LIGHT);

    table.SelectRow(0).BorderBottom(LIGHT);

    table.SelectAll().Decorate(color(Color::Green));

    table.SelectColumn(1).Decorate(color(Color::White));

    auto document = table.Render();
    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}
