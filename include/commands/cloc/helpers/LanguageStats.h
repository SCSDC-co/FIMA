/*
 * src/commands/cloc/helpers/LanguageStats.cpp
 * include/commands/cloc/helpers/LanguageStats.h
 *
 * Declaration of the LanguageStats class
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include "commands/cloc/helpers/Stats.h"

namespace fima {

namespace cloc {

namespace classes {

class LanguageStats
{
  private:
    int files{ 1 }; // a language cannot exists without at least one file

  public:
    Stats stats{};

    LanguageStats(const int& _code = 0, const int& _blank_lines = 0, const int& _comments = 0)
      : stats(_code, _blank_lines, _comments) {};

    void update_files();

    [[nodiscard]] int get_files() const;
};

} // namespace classes

} // namespace cloc

} // namespace fima
