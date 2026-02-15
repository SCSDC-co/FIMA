/*
 * src/commands/cloc/helpers/LanguageStats.cpp
 * include/commands/cloc/helpers/LanguageStats.h
 *
 * Declaration of the LanguageStats class
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include "commands/cloc/helpers/FileStats.h"

namespace fima {

namespace cloc {

namespace classes {

class LanguageStats : public FileStats
{
  private:
    int files{ 0 };

  public:
    void update_files();

    [[nodiscard]] int get_files() const;
};

}

}

}
