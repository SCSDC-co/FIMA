#include "commands/cloc/cloc.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/language_file.h"
#include "commands/cloc/helpers/language_map.h"
#include "helpers/colors.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

using json = nlohmann::json;

void
main(fs::path path)
{
    json languages_file = helpers::get_languages_file();

    std::string file_extension       = path.extension();
    std::string file_language_family = helpers::get_language_family(file_extension);
    std::string file_language_name   = helpers::get_language_name(file_extension);

    helpers::FileStats file_stats = helpers::count_lines(path);

    std::cout << fima::colors::GREEN << "File: " << fima::colors::RESET << path.string() << '\n';
    std::cout << fima::colors::GREEN << "Language name: " << fima::colors::RESET
              << file_language_name << '\n';
    std::cout << fima::colors::GREEN << "Language family: " << fima::colors::RESET
              << file_language_family << '\n';
    std::cout << fima::colors::GREEN << "Lines of code: " << fima::colors::RESET
              << file_stats.get_total() << '\n';
}

}

}
