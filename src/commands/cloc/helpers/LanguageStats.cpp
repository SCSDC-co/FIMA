#include "commands/cloc/helpers/LanguageStats.h"

namespace fima {

namespace cloc {

namespace classes {

void
LanguageStats::update_files()
{
    ++this->files;
};

[[nodiscard]] int
LanguageStats::get_files() const
{
    return this->files;
};

}

}

}
