#include "../../../include/commands/cloc/cloc.h"

#include <filesystem>
#include <iostream>
#include <vector>

#include "../../../include/commands/cloc/helpers/file_stats.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void start(std::vector<fs::path> path) {
    fima::cloc::FileStats stats;

    for (const auto &entry : path) {
        stats.process(entry);

        stats.print();

        if (&entry != &*path.rbegin()) {
            std::cout << '\n';
        }
    }
}

} // namespace cloc

} // namespace fima
