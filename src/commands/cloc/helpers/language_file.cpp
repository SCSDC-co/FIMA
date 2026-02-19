/*
 * src/commands/cloc/helpers/language_file.cpp
 * include/commands/cloc/helpers/language_file.h
 *
 * This file has the logic for the language file
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/cloc/helpers/language_file.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "helpers/get_data_path.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

namespace helpers {

using json = nlohmann::json;

[[nodiscard]] json
get_languages_file()
{
    json file;

    fs::path data_path          = fima::helpers::get_application_data_path();
    fs::path fima_data_path     = data_path / "fima";
    fs::path language_file_path = fima_data_path / "languages.json";

    std::ifstream file_stream(language_file_path);
    file = json::parse(file_stream);

    return file;
}

} // namespace helpers

} // namespace cloc

} // namespace fima
