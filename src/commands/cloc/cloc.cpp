#include "commands/cloc/cloc.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

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

    std::optional<std::string> single_comment =
      languages_file[file_language_family]["comments"]["single"].get<std::string>();
    std::optional<std::string> multiline_start =
      languages_file[file_language_family]["comments"]["multiline_start"].get<std::string>();
    std::optional<std::string> multiline_end =
      languages_file[file_language_family]["comments"]["multiline_end"].get<std::string>();

    helpers::FileStats file_stats =
      helpers::count_lines(path, single_comment, multiline_start, multiline_end);

    std::cout << fima::colors::GREEN << "File: " << fima::colors::RESET << path.string()
              << fima::colors::GREEN << " (" << file_language_name << ")" << fima::colors::RESET
              << '\n';

    std::cout << fima::colors::GREEN << "──────";

    for (int i = 0; i < path.string().length(); ++i) {
        std::cout << "─";
    }

    std::cout << "──";

    for (int i = 0; i < file_language_name.length(); ++i) {
        std::cout << "─";
    }

    std::cout << "─";

    std::cout << fima::colors::RESET << '\n';

    std::cout << fima::colors::GREEN << "Lines of code: " << fima::colors::RESET
              << file_stats.get_code() << '\n';
    std::cout << fima::colors::GREEN << "Lines of comments: " << fima::colors::RESET
              << file_stats.get_comment() << '\n';
    std::cout << fima::colors::GREEN << "Blank lines: " << fima::colors::RESET
              << file_stats.get_blank() << '\n';
    std::cout << fima::colors::GREEN << "Total lines: " << fima::colors::RESET
              << file_stats.get_total() << '\n';
}

}

}
