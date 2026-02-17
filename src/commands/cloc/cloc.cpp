/*
 * src/commands/cloc/cloc.cpp
 * include/commands/cloc/cloc.h
 *
 * The main entry point of the `cloc` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/cloc/cloc.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
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
#include "config.h"
#include "helpers/get_directories_entries.h"
#include "utility/colors.h"
#include "utility/regex.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

void
cloc(const std::vector<fs::path>& paths,
     const std::vector<std::regex>& paths_to_ignore,
     const std::string& sorting,
     const bool& quiet)
{
    const std::unordered_set<std::string> sorting_options = {
        "files", "total", "code", "comments", "blank"
    };

    if (!sorting_options.contains(sorting)) {
        std::cout << fima::colors::RED << "This sorting option: " << fima::colors::RESET << sorting
                  << fima::colors::RED << " doesn't exists." << fima::colors::RESET << '\n';

        std::cout << fima::colors::RED << "Available options:" << fima::colors::RESET << '\n';

        for (const std::string& item : sorting_options) {
            std::cout << "  " << item << (item == "total" ? " (default)" : "") << '\n';
        }

        return;
    }

    using json = nlohmann::json;

    json languages_file = helpers::get_languages_file();

    std::unordered_map<std::string, fima::cloc::classes::LanguageStats> analyzed_languages{};

    std::vector<fs::path> paths_all       = {};
    std::vector<fs::path> sanitized_paths = {};

    std::vector<std::regex> file_or_directories_to_ignore = fima::config::DEFAULT_DIRS_TO_IGNORE;

    for (const std::regex& item : paths_to_ignore) {
        file_or_directories_to_ignore.push_back(item);
    }

    for (const fs::path& path : paths) {
        if (fs::is_directory(path)) {
            std::vector<fs::path> items = fima::helpers::get_directories_entries_recursive(
              path, true, file_or_directories_to_ignore);

            for (const auto& item : items) {
                paths_all.push_back(item);
            }
        } else {
            paths_all.push_back(path);
        }
    }

    static const std::vector<std::regex> ft_to_skip = {
        std::regex{ R"(.*\.zip)" },   std::regex{ R"(.*\.tar)" },   std::regex{ R"(.*\.png)" },
        std::regex{ R"(.*\.jpeg)" },  std::regex{ R"(.*\.jpg)" },   std::regex{ R"(.*\.mp3)" },
        std::regex{ R"(.*\.mp4)" },   std::regex{ R"(.*\.mp2)" },   std::regex{ R"(.*\.mp1)" },
        std::regex{ R"(.*\.wav)" },   std::regex{ R"(.*\.avi)" },   std::regex{ R"(.*\.webp)" },
        std::regex{ R"(.*\.undo)" },  std::regex{ R"(.*\.spl)" },   std::regex{ R"(.*\.ico)" },
        std::regex{ R"(.*\.icns)" },  std::regex{ R"(.*\.mpack)" }, std::regex{ R"(.*\.exe)" },
        std::regex{ R"(.*\.o)" },     std::regex{ R"(.*\.class)" }, std::regex{ R"(.*\.appimage)" },
        std::regex{ R"(.*\.woff2)" }, std::regex{ R"(.*\.ttf)" },   std::regex{ R"(.*\.ttf2)" },
        std::regex{ R"(.*\.dll)" }
    };

    for (const fs::path& path : paths_all) {
        if (fs::is_directory(path)) {
            continue;
        }

        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (fima::helpers::regex::matches_any_regex(ext, ft_to_skip)) {
            continue;
        }

        if (fima::helpers::regex::matches_any_regex(path.string(), file_or_directories_to_ignore)) {
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

    fima::cloc::helpers::print_table(analyzed_languages, sorting, quiet);
}

void
main(const std::vector<fs::path>& paths,
     const bool& show_languages,
     const std::vector<std::regex>& paths_to_ignore,
     const std::string& sorting,
     const bool& quiet)
{
    if (show_languages) {
        fima::cloc::helpers::show_languages();
    } else {
        cloc(paths, paths_to_ignore, sorting, quiet);
    }
}

} // namespace cloc

} // namespace fima
