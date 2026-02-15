/*
 * src/commands/cloc/helpers/LanguageStats.cpp
 * include/commands/cloc/helpers/LanguageStats.h
 *
 * Implementation of the LanguageStats class
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

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
