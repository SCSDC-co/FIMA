#include "commands/cloc/cloc.h"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "commands/cloc/helpers/FileStats.h"
#include "commands/cloc/helpers/LanguageStats.h"
#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/language_file.h"
#include "commands/cloc/helpers/language_map.h"
#include "commands/cloc/helpers/print_table.h"
#include "helpers/get_directories_entries.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

using json = nlohmann::json;

void
main(const std::vector<fs::path>& paths)
{
    json languages_file = helpers::get_languages_file();

    std::unordered_map<std::string, fima::cloc::classes::LanguageStats> analyzed_languages;

    std::vector<fs::path> paths_all = {};

    for (const fs::path& path : paths) {
        if (fs::is_directory(path)) {
            std::vector<fs::path> items = fima::helpers::get_directories_entries_recursive(path);

            for (const fs::path& item : items) {
                if (!fs::is_directory(item)) {
                    paths_all.push_back(item);
                }
            }
        } else {
            paths_all.push_back(path);
        }
    }

    for (const fs::path& path : paths_all) {
        std::string file_extension       = path.extension();
        std::string file_language_family = helpers::get_language_family(file_extension);
        std::string file_language_name   = helpers::get_language_name(file_extension);

        std::string single_comment =
          languages_file[file_language_family]["comments"]["single"].get<std::string>();
        std::string multiline_start =
          languages_file[file_language_family]["comments"]["multiline_start"].get<std::string>();
        std::string multiline_end =
          languages_file[file_language_family]["comments"]["multiline_end"].get<std::string>();

        fima::cloc::classes::FileStats file_stats =
          helpers::count_lines(path, single_comment, multiline_start, multiline_end);

        fima::cloc::classes::LanguageStats language_stats;

        language_stats.set_code(file_stats.get_code());
        language_stats.set_blank(file_stats.get_blank());
        language_stats.set_comment(file_stats.get_comment());
        language_stats.set_total();
        language_stats.update_files();

        auto language = analyzed_languages.find(file_language_name);

        if (language == analyzed_languages.end()) {
            analyzed_languages.insert({ file_language_name, language_stats });
        } else {
            analyzed_languages.at(file_language_name)
              .set_code(analyzed_languages.at(file_language_name).get_code() +
                        language_stats.get_code());

            analyzed_languages.at(file_language_name)
              .set_blank(analyzed_languages.at(file_language_name).get_blank() +
                         language_stats.get_blank());

            analyzed_languages.at(file_language_name)
              .set_comment(analyzed_languages.at(file_language_name).get_comment() +
                           language_stats.get_comment());

            analyzed_languages.at(file_language_name).set_total();

            analyzed_languages.at(file_language_name).update_files();
        }
    }

    fima::cloc::helpers::print_table(analyzed_languages);
}

}

}
