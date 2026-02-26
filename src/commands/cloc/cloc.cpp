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

#include "commands/cloc/helpers/LanguageStats.h"
#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/print_languages.h"
#include "commands/cloc/helpers/print_table.h"
#include "config.h"
#include "fs/get_directories_entries.h"
#include "logger.h"
#include "options.h"
#include "program_files.h"
#include "utility/colors.h"
#include "utility/regex.h"

namespace _fs = std::filesystem;

namespace fima {

namespace cloc {

void
cloc(const std::vector<std::regex>& paths_to_ignore, const fima::options::cloc_options& options)
{
    const std::unordered_set<std::string> sorting_options = {
        "files", "total", "code", "comments", "blank"
    };

    if (!sorting_options.contains(options.sorting)) {
        std::cout << fima::colors::RED << "This sorting option: " << fima::colors::RESET
                  << options.sorting << fima::colors::RED << " doesn't exists."
                  << fima::colors::RESET << '\n';

        std::cout << fima::colors::RED << "Available options:" << fima::colors::RESET << '\n';

        for (const std::string& item : sorting_options) {
            std::cout << "  " << item << (item == "total" ? " (default)" : "") << '\n';
        }

        return;
    }

    std::unordered_map<std::string, fima::cloc::classes::LanguageStats> analyzed_languages{};

    std::vector<_fs::path> paths_all       = {};
    std::vector<_fs::path> sanitized_paths = {};

    std::vector<std::regex> file_or_directories_to_ignore = fima::config::DEFAULT_DIRS_TO_IGNORE;

    for (const std::regex& item : paths_to_ignore) {
        file_or_directories_to_ignore.push_back(item);
    }

    for (const _fs::path& path : options.paths) {
        if (_fs::is_directory(path)) {
            std::vector<_fs::path> items = fima::fs::get_directories_entries_recursive(
              path, true, file_or_directories_to_ignore);

            for (const auto& item : items) {
                if (!_fs::is_regular_file(path)) {
                    continue;
                }

                paths_all.push_back(item);
            }
        } else if (!_fs::is_regular_file(path)) {
            continue;
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

    for (const _fs::path& path : paths_all) {
        if (_fs::is_directory(path)) {
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

    for (const _fs::path& path : sanitized_paths) {
        std::string file_language_family = fima::program_files::get_language_family(path);
        std::string file_language_name   = fima::program_files::get_language_name(path);

        std::string single_comment =
          fima::program_files::language_file_json[file_language_family]["comments"]["single"]
            .get<std::string>();
        std::string multiline_start =
          fima::program_files::language_file_json[file_language_family]["comments"]
                                                 ["multiline_start"]
                                                   .get<std::string>();
        std::string multiline_end =
          fima::program_files::language_file_json[file_language_family]["comments"]["multiline_end"]
            .get<std::string>();

        fima::cloc::classes::Stats file_stats =
          helpers::count_lines(path, single_comment, multiline_start, multiline_end);

        fima::cloc::classes::LanguageStats language_stats;

        language_stats.set_code(file_stats.get_code());
        language_stats.set_blank_lines(file_stats.get_blank_lines());
        language_stats.set_comments(file_stats.get_comments());
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
              .set_blank_lines(analyzed_languages.at(file_language_name).get_blank_lines() +
                               language_stats.get_blank_lines());

            analyzed_languages.at(file_language_name)
              .set_comments(analyzed_languages.at(file_language_name).get_comments() +
                            language_stats.get_comments());

            analyzed_languages.at(file_language_name).set_total();

            analyzed_languages.at(file_language_name).update_files();
        }
    }

    fima::cloc::helpers::print_table(analyzed_languages, options.sorting, options.quiet);
}

void
main(const std::vector<std::regex>& paths_to_ignore, const fima::options::cloc_options options)
{
    if (options.show_languages) {
        fima::cloc::helpers::show_languages();
    } else {
        cloc(paths_to_ignore, options);
    }

    std::string paths_sanitized{};
    std::string paths_to_ignore_sanitized{};

    for (const _fs::path& item : options.paths) {
        paths_sanitized += item.string() + ", ";
    }

    fima::logger::info(false, "cloc", "Got loc of: {}", paths_sanitized);
    fima::logger::info(false, "cloc", "Options:");
    fima::logger::info(false, "cloc", "  Quiet: {}", (options.quiet ? "true" : "false"));
    fima::logger::info(false, "cloc", "  Sorting: {}", options.sorting);
    fima::logger::info(
      false, "cloc", "  Show langauges: {}", (options.show_languages ? "true" : "false"));
}

} // namespace cloc

} // namespace fima
