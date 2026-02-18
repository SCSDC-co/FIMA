/*
 * src/commands/permissions.cpp
 * include/commands/permissions.h
 *
 * The declaration of the `permissions` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <ftxui/dom/node.hpp>
#include <vector>

namespace fima {

namespace perms {

std::string
get_perms(const std::filesystem::path& item);

ftxui::Element
get_perms_tui(const std::filesystem::path& item);

void
persmissions(const std::vector<std::filesystem::path>& paths);

} // namespace perms

} // namespace fima
