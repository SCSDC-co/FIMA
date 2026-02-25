/*
 * src/program_files.cpp
 * include/program_files.h
 *
 * A utility to setup the program files (like the config file, and the JSON files)
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include <filesystem>
#include <string_view>

#include "fs/get_config_path.h"

namespace fima {

namespace program_files {

inline const std::string_view languages_file = R"json(
{
    "c_like": {
        "comments": {
            "single": "//",
            "multiline_start": "/*",
            "multiline_end": "*/"
        }
    },
    "lisp_like": {
        "comments": {
            "single": ";",
            "multiline_start": "",
            "multiline_end": ""
        }
    },
    "shell_like": {
        "comments": {
            "single": "#",
            "multiline_start": "",
            "multiline_end": ""
        }
    },
    "markup": {
        "comments": {
            "single": "",
            "multiline_start": "<!--",
            "multiline_end": "-->"
        }
    },
    "python": {
        "comments": {
            "single": "#",
            "multiline_start": "\"\"\"",
            "multiline_end": "\"\"\""
        }
    },
    "css": {
        "comments": {
            "single": "",
            "multiline_start": "/*",
            "multiline_end": "*/"
        }
    },
    "text": {
        "comments": {
            "single": "",
            "multiline_start": "",
            "multiline_end": ""
        }
    },
    "vim": {
        "comments": {
            "single": "\"",
            "multiline_start": "",
            "multiline_end": ""
        }
    },
    "cmd": {
        "comments": {
            "single": "::",
            "multiline_start": "",
            "multiline_end": ""
        }
    },
    "lua": {
        "comments": {
            "single": "--",
            "multiline_start": "--[[",
            "multiline_end": "--]]"
        }
    },
    "react": {
        "comments": {
            "single": "",
            "multiline_start": "{/*",
            "multiline_end": "*/}"
        }
    }
}
)json";

inline const std::string_view map_languages_name = R"json(
{
    ".cpp": "C++",
    ".cxx": "C++",
    ".cc": "C++",
    ".c": "C",
    ".h": "C Header",
    ".hpp": "C++ Header",
    ".hxx": "C++ Header",
    ".inl": "C++ Header",
    ".rs": "Rust",
    ".cs": "C#",
    ".js": "JavaScript",
    ".ts": "TypeScript",
    ".mjs": "JavaScript (ESM)",
    ".mts": "TypeScript (ESM)",
    ".cjs": "JavaScript (CJS)",
    ".cts": "TypeScript (CJS)",
    ".java": "Java",
    ".m": "Objective-C",
    ".php": "PHP",
    ".dart": "Dart",
    ".go": "Go",
    ".swift": "Swift",
    ".kt": "Kotlin",
    ".scala": "Scala",
    ".hx": "Haxe",
    ".hxml": "Haxe",
    ".zig": "Zig",
    ".zon": "ZON",
    ".jsonc": "JSONC",
    ".rc": "RC",
    ".lisp": "Common Lisp",
    ".lsp": "Common Lisp",
    ".cl": "Common Lisp",
    ".scm": "Scheme",
    ".rkt": "Racket",
    ".clj": "Clojure",
    ".cljs": "Clojure",
    ".cljc": "Clojure",
    ".edn": "EDN",
    ".el": "Emacs Lisp",
    ".hy": "Hy",
    ".janet": "Janet",
    ".fnl": "Fennel",
    ".asm": "Assembly",
    ".s": "Assembly",
    ".sh": "Bash",
    ".zsh": "ZSH",
    ".fish": "Fish",
    ".dash": "Dash",
    ".ksh": "KSH",
    ".ash": "ASH",
    ".tcsh": "TCSH",
    ".csh": "CSH",
    ".rb": "Ruby",
    ".pl": "PL/I",
    ".pm": "PM",
    ".raku": "Raku",
    ".rakumod": "Raku",
    ".p6": "P6",
    ".jl": "Julia",
    ".nim": "Nim",
    ".cr": "Crystal",
    ".tf": "Tensor Flow",
    ".nix": "NIX",
    ".sls": "S/LS",
    ".cmake": "CMake",
    ".yaml": "YAML",
    ".yml": "YAML",
    ".toml": "TOML",
    ".ini": "INI",
    ".inp": "INP",
    ".ps1": "PWSH Script",
    ".bat": "CMD",
    ".mak": "Make",
    ".desktop": "Desktop File",
    ".cfg": "CFG",
    ".conf": "Config File",
    ".html": "HTML",
    ".htm": "HTML",
    ".xhtml": "XHTML",
    ".md": "Markdown",
    ".markdown": "Markdown",
    ".xml": "XML",
    ".xaml": "XAML",
    ".svg": "SVG",
    ".rst": "RST",
    ".tex": "TeX",
    ".adoc": "ASCII Doc",
    ".wxs": "WIX",
    ".manifest": "Manifest",
    ".csproj": "C# Project",
    ".wixproj": "WIX Project",
    ".txt": "Text",
    ".json": "JSON",
    ".tutor": "Tutor",
    ".in": "IN",
    ".ti": "TI",
    ".supp": "SUPP",
    ".ok": "Text",
    ".example": "Text",
    ".dict": "Dictionary",
    ".cat": "Text",
    ".1": "Man",
    ".lock": "Lock File",
    ".log": "Log",
    ".py": "Python",
    ".css": "CSS",
    ".scss": "CSS",
    ".sass": "CSS",
    ".less": "CSS",
    ".styl": "CSS",
    ".stylus": "CSS",
    ".pcss": "CSS",
    ".vim": "VIM",
    ".lua": "Lua",
    ".cmd": "CMD",
    ".jsx": "JavaScript React",
    ".tsx": "TypeScript React"
}
)json";

inline const std::string_view map_languages_family = R"json(
{
    ".cpp": "c_like",
    ".cxx": "c_like",
    ".cc": "c_like",
    ".c": "c_like",
    ".h": "c_like",
    ".hpp": "c_like",
    ".hxx": "c_like",
    ".inl": "c_like",
    ".rs": "c_like",
    ".cs": "c_like",
    ".js": "c_like",
    ".ts": "c_like",
    ".mjs": "c_like",
    ".mts": "c_like",
    ".cjs": "c_like",
    ".cts": "c_like",
    ".java": "c_like",
    ".m": "c_like",
    ".php": "c_like",
    ".dart": "c_like",
    ".go": "c_like",
    ".swift": "c_like",
    ".kt": "c_like",
    ".scala": "c_like",
    ".hx": "c_like",
    ".hxml": "c_like",
    ".zig": "c_like",
    ".zon": "c_like",
    ".jsonc": "c_like",
    ".rc": "c_like",
    ".lisp": "lisp_like",
    ".lsp": "lisp_like",
    ".cl": "lisp_like",
    ".scm": "lisp_like",
    ".rkt": "lisp_like",
    ".clj": "lisp_like",
    ".cljs": "lisp_like",
    ".cljc": "lisp_like",
    ".edn": "lisp_like",
    ".el": "lisp_like",
    ".ss": "lisp_like",
    ".hy": "lisp_like",
    ".janet": "lisp_like",
    ".fnl": "lisp_like",
    ".asm": "lisp_like",
    ".s": "lisp_like",
    ".sh": "shell_like",
    ".zsh": "shell_like",
    ".fish": "shell_like",
    ".dash": "shell_like",
    ".ksh": "shell_like",
    ".ash": "shell_like",
    ".tcsh": "shell_like",
    ".csh": "shell_like",
    ".rb": "shell_like",
    ".pl": "shell_like",
    ".pm": "shell_like",
    ".raku": "shell_like",
    ".rakumod": "shell_like",
    ".p6": "shell_like",
    ".jl": "shell_like",
    ".nim": "shell_like",
    ".cr": "shell_like",
    ".tf": "shell_like",
    ".nix": "shell_like",
    ".sls": "shell_like",
    ".cmake": "shell_like",
    ".yaml": "shell_like",
    ".yml": "shell_like",
    ".toml": "shell_like",
    ".ini": "shell_like",
    ".inp": "shell_like",
    ".ps1": "shell_like",
    ".bat": "shell_like",
    ".mak": "shell_like",
    ".desktop": "shell_like",
    ".cfg": "shell_like",
    ".conf": "shell_like",
    ".html": "markup",
    ".htm": "markup",
    ".xhtml": "markup",
    ".md": "markup",
    ".markdown": "markup",
    ".xml": "markup",
    ".xaml": "markup",
    ".svg": "markup",
    ".rst": "markup",
    ".tex": "markup",
    ".adoc": "markup",
    ".wxs": "markup",
    ".manifest": "markup",
    ".csproj": "markup",
    ".wixproj": "markup",
    ".txt": "text",
    ".json": "text",
    ".tutor": "text",
    ".in": "text",
    ".ti": "text",
    ".supp": "text",
    ".ok": "text",
    ".example": "text",
    ".dict": "text",
    ".cat": "text",
    ".1": "text",
    ".lock": "text",
    ".log": "text",
    ".py": "python",
    ".css": "css",
    ".scss": "css",
    ".sass": "css",
    ".less": "css",
    ".styl": "css",
    ".stylus": "css",
    ".pcss": "css",
    ".vim": "vim",
    ".lua": "lua",
    ".cmd": "cmd",
    ".jsx": "react",
    ".tsx": "react"
}
)json";

inline const std::string_view map_languages_icon = R"json(
{
    ".cpp": "",
    ".cxx": "",
    ".cc": "",
    ".c": "",
    ".h": "",
    ".hpp": "",
    ".hxx": "",
    ".inl": "",
    ".rs": "",
    ".cs": "",
    ".js": "",
    ".ts": "",
    ".mjs": "",
    ".mts": "",
    ".cjs": "",
    ".cts": "",
    ".java": "",
    ".m": "",
    ".php": "",
    ".dart": "",
    ".go": "",
    ".swift": "",
    ".kt": "",
    ".scala": "",
    ".hx": "",
    ".hxml": "",
    ".zig": "",
    ".zon": "",
    ".jsonc": "",
    ".lisp": "",
    ".lsp": "",
    ".cl": "",
    ".scm": "",
    ".clj": "",
    ".cljs": "",
    ".cljc": "",
    ".el": "",
    ".fnl": "",
    ".asm": "",
    ".s": "",
    ".sh": "",
    ".zsh": "",
    ".fish": "",
    ".dash": "",
    ".ksh": "",
    ".ash": "",
    ".tcsh": "",
    ".csh": "",
    ".rb": "",
    ".jl": "",
    ".nim": "",
    ".cr": "",
    ".tf": "",
    ".nix": "󱄅",
    ".cmake": "",
    ".yaml": "",
    ".yml": "",
    ".toml": "",
    ".ini": "",
    ".ps1": "",
    ".bat": "",
    ".mak": "",
    ".desktop": "",
    ".conf": "",
    ".html": "",
    ".htm": "",
    ".xhtml": "",
    ".md": "",
    ".markdown": "",
    ".xml": "󰗀",
    ".xaml": "󰙳",
    ".svg": "󰜡",
    ".tex": "",
    ".wxs": "",
    ".csproj": "",
    ".wixproj": "",
    ".json": "",
    ".1": "",
    ".lock": "",
    ".log": "",
    ".py": "",
    ".css": "",
    ".scss": "",
    ".sass": "",
    ".less": "",
    ".styl": "",
    ".stylus": "",
    ".pcss": "",
    ".vim": "",
    ".lua": "",
    ".cmd": "",
    ".jsx": "",
    ".tsx": ""
}
)json";

inline const std::string_view map_directory_icon = R"json(
{
    ".git": "",
    "include": "󱁿",
    "src": "󰣞",
    "node_modules": "",
    ".config": "󱁿",
    ".vscode": "󰨞",
    ".idea": "",
    "cmake": "",
    "scripts": "󰴉",
    "docs": "󱂷",
    "doc": "󱂷",
    "assets": "󰉏",
    "res": "󰉏",
    "resources": "󰉏",
    "public": "󰙉",
    "test": "󰙨",
    "tests": "󰙨",
    "__tests__": "󰙨",
    "debug": "󰃤",
    "benchmark": "󰅒",
    "api": "󱂛",
    "client": "󱩛",
    "server": "",
    ".github": ""
}
)json";

inline const std::filesystem::path CONFIG_PATH      = fima::fs::get_application_config_path();
inline const std::filesystem::path FIMA_CONFIG_PATH = CONFIG_PATH / "fima";
inline const std::filesystem::path MAPPINGS_PATH    = FIMA_CONFIG_PATH / "mappings";

inline const std::filesystem::path MAP_LANGUAGES_NAME_PATH = MAPPINGS_PATH / "language_name.json";
inline const std::filesystem::path MAP_LANGUAGES_FAMILY_PATH =
  MAPPINGS_PATH / "language_family.json";
inline const std::filesystem::path MAP_LANGUAGES_ICON_PATH = MAPPINGS_PATH / "lanugage_icon.json";
inline const std::filesystem::path MAP_DIRECTORY_ICON_PATH = MAPPINGS_PATH / "directory_icon.json";
inline const std::filesystem::path LANGUAGES_FILE_PATH = FIMA_CONFIG_PATH / "languages_specs.json";
inline const std::filesystem::path CONFIG_FILE_PATH    = FIMA_CONFIG_PATH / "config.toml";

void
create_config_files();

void
reset_config_files(const bool& preserve_config_file);

} // namespace program_files

} // namespace fima
