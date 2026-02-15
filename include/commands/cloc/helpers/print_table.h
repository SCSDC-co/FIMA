/*
 * src/commands/cloc/helpers/print_table.cpp
 * include/commands/cloc/helpers/print_table.h
 *
 * This is the declaration of the logic for printing the cloc table
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <string>
#include <unordered_map>

#include "commands/cloc/helpers/LanguageStats.h"

namespace fima {

namespace cloc {

namespace helpers {

void
print_table(const std::unordered_map<std::string, fima::cloc::classes::LanguageStats>& language_map,
            const std::string& sorting);

}

}

}
