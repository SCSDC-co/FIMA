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

#include "utility/FileInfo.h"

namespace fima {

namespace ls {

namespace helpers {

void
print_normal(const std::vector<fima::FileInfo>& items, const bool& icons);

void
print_long(const std::vector<fima::FileInfo>& files, const bool& icons);

} // namespace helpers

} // namespace ls

} // namespace fima
