#include "../../../include/commands/cloc/cloc.h"

#include <filesystem>

#include "../../../include/commands/cloc/helpers/file_stats.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void start(fs::path path) {
    fima::cloc::FileStats stats;

    stats.process(path);

    stats.print();
}

} // namespace cloc

} // namespace fima
