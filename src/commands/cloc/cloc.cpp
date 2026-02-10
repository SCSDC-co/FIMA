#include "commands/cloc/cloc.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

#include "commands/cloc/helpers/language_file.h"
#include "commands/cloc/helpers/language_map.h"

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

    std::cout << path.string() << " extension: " << file_extension << '\n';
    std::cout << path.string() << " language family: " << file_language_family << '\n';
    std::cout << path.string() << " comments:" << '\n';
    std::cout << "  single: "
              << languages_file[file_language_family]["comments"]["single"].get<std::string>()
              << '\n';
    std::cout
      << "  multiline start: "
      << languages_file[file_language_family]["comments"]["multiline_start"].get<std::string>()
      << '\n';
    std::cout
      << "  multiline end: "
      << languages_file[file_language_family]["comments"]["multiline_end"].get<std::string>()
      << '\n';
}

}

}
