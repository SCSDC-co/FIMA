/*
 * src/commands/ls/helpers/printer.cpp
 * include/commands/ls/helpers/printer.h
 *
 * An helper to print the directory entries
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <vector>

#include "fs/DirectoryItem.h"

namespace fima {

namespace ls {

namespace helpers {

void
print_normal(const std::vector<fima::fs::DirectoryItem>& items,
             const bool& icons,
             const bool& relative_paths);

void
print_one_line(const std::vector<fima::fs::DirectoryItem>& items,
               const bool& icons,
               const bool& relative_paths);

void
print_long(std::vector<fima::fs::DirectoryItem>& items,
           const bool& icons,
           const bool& verbose,
           const bool& headers,
           const bool& relative_paths);

} // namespace helpers

} // namespace ls

} // namespace fima
