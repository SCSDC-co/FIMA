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

#include <fstream>
#include <nlohmann/json.hpp>

#include "program_files.h"

namespace fima {

namespace cloc {

namespace helpers {

using json = nlohmann::json;

[[nodiscard]] json
get_languages_file()
{
    json file;

    std::ifstream file_stream(fima::program_files::LANGUAGES_FILE_PATH);
    file = json::parse(file_stream);

    return file;
}

} // namespace helpers

} // namespace cloc

} // namespace fima
