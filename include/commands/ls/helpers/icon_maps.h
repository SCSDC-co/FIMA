/*
 * src/commands/helpers/icon_maps.cpp
 * include/commands/helpers/icon_maps.h
 *
 * An helper to get the correct icon for each file/directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>

namespace fima {

namespace ls {

namespace helpers {

[[nodiscard]] std::string
get_item_icon(const std::filesystem::path& path);

} // namespace helpers

} // namespace ls

} // namespace fima
