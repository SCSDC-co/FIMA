#include "../../../include/commands/cloc/cloc.h"

#include <filesystem>

#include "../../../include/commands/cloc/helpers/count_lines.h"
#include "../../../include/commands/cloc/helpers/print.h"
#include "../../../include/commands/cloc/helpers/types.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void start(fs::path path) {
    fima::cloc::FileStats file;

    file = fima::cloc::cout_lines(path);

    fima::cloc::print(file);
}

} // namespace cloc

} // namespace fima
