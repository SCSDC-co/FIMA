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
#include "fs/operations.h"
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
                if (!_fs::is_regular_file(item)) {
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

    static const std::unordered_set<std::string> ft_to_skip = {
        ".zip",  ".tar",    ".png",      ".jpeg",  ".jpg",  ".mp3",  ".mp4",  ".mp2",   ".mp1",
        ".wav",  ".avi",    ".webp",     ".undo",  ".spl",  ".ico",  ".icns", ".mpack", ".exe",
        ".o",    ".class",  ".appimage", ".woff2", ".ttf",  ".ttf2", ".dll",  ".rar",   ".7z",
        ".gz",   ".bz2",    ".xz",       ".z",     ".lz",   ".lzma", ".lzo",  ".zst",   ".tgz",
        ".tbz2", ".tar.gz", ".tar.xz",   ".gif",   ".bmp",  ".tiff", ".tif",  ".avif",  ".heif",
        ".heic", ".svg",    ".psd",      ".xcf",   ".raw",  ".cr2",  ".nef",  ".arw",   ".dng",
        ".mkv",  ".webm",   ".mov",      ".wmv",   ".flv",  ".mpeg", ".mpg",  ".m4v",   ".3gp",
        ".m2ts", ".mts",    ".vob",      ".ogv",   ".rmvb", ".hevc", ".h264", ".h265",  ".flac",
        ".aac",  ".m4a",    ".ogg",      ".opus",  ".wma",  ".aiff", ".alac", ".ape",   ".mid",
        ".midi", ".so",     ".dylib",    ".a",     ".obj",  ".lib",  ".bin",  ".out",   ".elf",
        ".pdb",  ".deb",    ".rpm",      ".snap",  ".msi",  ".pkg",  ".dmg",  ".apk",   ".jar",
        ".war",  ".ear",    ".iso",      ".md5",   ".pdf",  ".gtk",  ".gtk1", ".gtk2",  ".gtk3",
        ".gtk4", ".gtk5",   ".gtk6"
    };

    for (const _fs::path& path : paths_all) {
        if (_fs::is_directory(path)) {
            continue;
        }

        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ft_to_skip.contains(ext)) {
            continue;
        }

        if (fima::helpers::regex::matches_any_regex(path.string(), file_or_directories_to_ignore)) {
            continue;
        }

        if (fima::fs::operations::is_file_executable(path)) {
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
