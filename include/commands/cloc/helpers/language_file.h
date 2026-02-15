/*
 * src/commands/cloc/helpers/language_file.cpp
 * include/commands/cloc/helpers/language_file.h
 *
 * This file is the declaration of the logic for the language file
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

namespace fima {

namespace cloc {

namespace helpers {

void
create_languages_file(std::filesystem::path path);

[[nodiscard]] nlohmann::json
get_languages_file();

}

}

}
