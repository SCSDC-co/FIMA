#include "../../../include/commands/cloc/cloc.h"

#include <filesystem>
#include <vector>

#include "../../../include/commands/cloc/helpers/file_stats.hpp"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void
start(std::vector<fs::directory_entry> path)
{
    fima::cloc::FileStats stats;

    for (const fs::directory_entry& entry : path) {
        stats.process(entry);
    }
}

} // namespace cloc

} // namespace fima
