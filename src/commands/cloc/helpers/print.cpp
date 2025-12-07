#include "../../../../include/commands/cloc/helpers/print.h"

#include <iostream>

#include "../../../../include/commands/cloc/helpers/types.h"

namespace fima {

namespace cloc {

void print(fima::cloc::FileStats file) {
    std::cout << "Number of lines of " << file.path.string() << '\n';
    std::cout << "Total   " << file.total << '\n';
    std::cout << "Blank   " << file.blank << '\n';
    std::cout << "Code    " << file.code << '\n';
    std::cout << "Comment " << file.comment << '\n';
}

} // namespace cloc

} // namespace fima
