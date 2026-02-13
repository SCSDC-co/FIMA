#pragma once

#include <string>
#include <unordered_map>

#include "commands/cloc/helpers/LanguageStats.h"

namespace fima {

namespace cloc {

namespace helpers {

void
print_table(std::unordered_map<std::string, fima::cloc::classes::LanguageStats> language_map);

}

}

}
