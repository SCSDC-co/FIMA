#include "commands/cloc/cloc.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/language_file.h"
#include "commands/cloc/helpers/language_map.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

using json = nlohmann::json;

void
main(const std::vector<fs::path>& paths)
{
    json languages_file = helpers::get_languages_file();

    // I know that helpers::FileStats doesn't have the best name for this purpose, but I think it's
    // useless to create a completely new class just for this purpose
    std::unordered_map<std::string, helpers::FileStats> analyzed_languages;

    for (const fs::path& path : paths) {
        std::string file_extension       = path.extension();
        std::string file_language_family = helpers::get_language_family(file_extension);
        std::string file_language_name   = helpers::get_language_name(file_extension);

        std::string single_comment =
          languages_file[file_language_family]["comments"]["single"].get<std::string>();
        std::string multiline_start =
          languages_file[file_language_family]["comments"]["multiline_start"].get<std::string>();
        std::string multiline_end =
          languages_file[file_language_family]["comments"]["multiline_end"].get<std::string>();

        helpers::FileStats file_stats =
          helpers::count_lines(path, single_comment, multiline_start, multiline_end);

        auto language = analyzed_languages.find(file_language_name);

        if (language == analyzed_languages.end()) {
            analyzed_languages.insert({ file_language_name, file_stats });
        } else {
            analyzed_languages.at(file_language_name) = file_stats;
        }
    }

    std::cout << '\n';

    for (const auto i : analyzed_languages) {
        std::cout << "Language: " << i.first << '\n';
        std::cout << "  Comments: " << i.second.get_comment() << '\n';
        std::cout << "  Blank: " << i.second.get_blank() << '\n';
        std::cout << "  Code: " << i.second.get_code() << '\n';
        std::cout << "  Total: " << i.second.get_total() << '\n';
        std::cout << '\n';
    }
}

}

}
