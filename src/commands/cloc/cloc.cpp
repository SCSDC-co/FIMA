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
#include <termcolor/termcolor.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "commands/cloc/helpers/LanguageStats.h"
#include "commands/cloc/helpers/count_lines.h"
#include "commands/cloc/helpers/print_languages.h"
#include "commands/cloc/helpers/print_table.h"
#include "config.h"
#include "fs/get_directories_entries.h"
#include "git/GitRepo.h"
#include "options.h"
#include "program_files.h"
#include "utility/regex.h"

namespace _fs = std::filesystem;

void
cloc(const std::vector<std::regex>& paths_to_ignore,
     const fima::git::GitRepo& repo,
     const fima::options::cloc_options& options)
{
    const std::unordered_set<std::string> sorting_options = {
        "files", "total", "code", "comments", "blank"
    };

    if (!sorting_options.contains(options.sorting)) {
        std::cout << termcolor::red << "This sorting option: " << termcolor::reset
                  << options.sorting << termcolor::red << " doesn't exists." << termcolor::reset
                  << '\n';

        std::cout << termcolor::red << "Available options:" << termcolor::reset << '\n';

        for (const std::string& item : sorting_options) {
            std::cout << "  " << item << (item == "total" ? " (default)" : "") << '\n';
        }

        return;
    }

    std::unordered_map<std::string, fima::cloc::classes::LanguageStats> analyzed_languages{};

    std::vector<_fs::path> paths = {};

    std::vector<std::regex> file_or_directories_to_ignore = fima::config::DEFAULT_DIRS_TO_IGNORE;

    file_or_directories_to_ignore.append_range(paths_to_ignore);

    for (const _fs::path& path : options.paths) {
        paths.append_range(fima::fs::get_directories_for_cloc(
          path, repo, options.gitignore, true, file_or_directories_to_ignore));
    }

    for (const _fs::path& path : paths) {
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
          fima::cloc::helpers::count_lines(path, single_comment, multiline_start, multiline_end);

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
_cloc(const std::vector<std::regex>& paths_to_ignore,
      const fima::git::GitRepo& repo,
      const fima::options::cloc_options options)
{
    if (options.show_languages) {
        fima::cloc::helpers::show_languages();
    } else {
        cloc(paths_to_ignore, repo, options);
    }

    std::string paths_sanitized{};
    std::string paths_to_ignore_sanitized{};

    for (const _fs::path& item : options.paths) {
        paths_sanitized += item.string() + ", ";
    }
}

namespace fima {

namespace commands {

void
setup_cloc(CLI::App& app, const fima::git::GitRepo& repo, fima::options::cloc_options& options)
{
    CLI::App* subcmd = app.add_subcommand("cloc", "Count lines of code")->configurable(false);

    subcmd->add_option("paths", options.paths, "The paths to work on (default current directory) ")
      ->configurable(false);

    subcmd->add_option("--ignore,-i", options.paths_to_ignore, "Paths to ignore")
      ->configurable(true)
      ->expected(0, -1);

    subcmd->add_option("--sort,-S", options.sorting, "Type of sorting")
      ->configurable(true)
      ->expected(0, 1);

    subcmd->add_flag("--quiet,-q", options.quiet, "Enables quiet output")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd
      ->add_flag(
        "--no-gitignore,-G", [&](int) { options.gitignore = false; }, "Ignore .gitignore")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    subcmd
      ->add_flag(
        "--show-languages,-s", options.show_languages, "Shows all the languages that cloc supports")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(false);

    subcmd->usage("fima cloc [PATHS] [OPTIONS]");

    subcmd->callback([&]() {
        std::vector<std::regex> regexes;

        for (const _fs::path& path : options.paths_to_ignore) {
            regexes.push_back(fima::helpers::regex::glob_to_regex(path.filename().string()));
        }

        _cloc(regexes, repo, options);
    });
}

} // namespace commands

} // namespace fima
