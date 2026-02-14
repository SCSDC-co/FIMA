#include "commands/cloc/cloc.h"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "commands/cloc/helpers/FileStats.h"
#include "commands/cloc/helpers/LanguageStats.h"
#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/language_file.h"
#include "commands/cloc/helpers/language_map.h"
#include "commands/cloc/helpers/print_languages.h"
#include "commands/cloc/helpers/print_table.h"
#include "helpers/get_directories_entries.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void
cloc(const std::vector<fs::path>& paths)
{
    using json = nlohmann::json;

    json languages_file = helpers::get_languages_file();

    std::unordered_map<std::string, fima::cloc::classes::LanguageStats> analyzed_languages;

    std::vector<fs::path> paths_all       = {};
    std::vector<fs::path> sanitized_paths = {};

    for (const fs::path& path : paths) {
        if (fs::is_directory(path)) {
            std::vector<fs::path> items =
              fima::helpers::get_directories_entries_recursive(path, true);

            for (const auto& item : items) {
                paths_all.push_back(item);
            }
        } else {
            paths_all.push_back(path);
        }
    }

    static const std::unordered_set<std::string> ft_to_skip = {
        ".zip",   ".tar", ".png", ".jpeg",  ".jpg",      ".mp3",   ".mp4", ".mp2",
        ".mp1",   ".wav", ".avi", ".webp",  ".undo",     ".spl",   ".ico", ".icns",
        ".mpack", ".exe", ".o",   ".class", ".appimage", ".woff2", ".ttf", ".ttf2"
    };

    for (const fs::path& path : paths_all) {
        if (fs::is_directory(path)) {
            continue;
        }

        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ft_to_skip.contains(ext)) {
            continue;
        }

        sanitized_paths.push_back(path);
    }

    for (const fs::path& path : sanitized_paths) {
        std::string file_language_family = helpers::get_language_family(path);
        std::string file_language_name   = helpers::get_language_name(path);

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

void
main(const std::vector<fs::path>& paths, const bool& show_languages)
{
    if (show_languages) {
        fima::cloc::helpers::show_languages();
    } else {
        cloc(paths);
    }
}

}

}
