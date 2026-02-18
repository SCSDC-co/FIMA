/*
 * src/commands/helpers/icon_maps.cpp
 * include/commands/helpers/icon_maps.h
 *
 * An helper to get the correct icon for each file/directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <filesystem>
#include <unordered_map>

namespace fima {

namespace ls {

namespace helpers {

inline const std::unordered_map<std::string, std::string> extension_icon_map = {
    { ".cpp", "" },      { ".cxx", "" },   { ".cc", "" },      { ".c", "" },
    { ".h", "" },        { ".hpp", "" },   { ".hxx", "" },     { ".inl", "" },
    { ".rs", "" },       { ".cs", "" },    { ".js", "" },      { ".ts", "" },
    { ".mjs", "" },      { ".mts", "" },   { ".cjs", "" },     { ".cts", "" },
    { ".java", "" },     { ".m", "" },     { ".php", "" },     { ".dart", "" },
    { ".go", "" },       { ".swift", "" }, { ".kt", "" },      { ".scala", "" },
    { ".hx", "" },       { ".hxml", "" },  { ".zig", "" },     { ".zon", "" },
    { ".jsonc", "" },    { ".lisp", "" },  { ".lsp", "" },     { ".cl", "" },
    { ".scm", "" },      { ".clj", "" },   { ".cljs", "" },    { ".cljc", "" },
    { ".el", "" },       { ".fnl", "" },   { ".asm", "" },     { ".s", "" },
    { ".sh", "" },       { ".zsh", "" },   { ".fish", "" },    { ".dash", "" },
    { ".ksh", "" },      { ".ash", "" },   { ".tcsh", "" },    { ".csh", "" },
    { ".rb", "" },       { ".jl", "" },    { ".nim", "" },     { ".cr", "" },
    { ".tf", "" },       { ".nix", "󱄅" },  { ".cmake", "" },   { ".yaml", "" },
    { ".yml", "" },      { ".toml", "" },  { ".ini", "" },     { ".ps1", "" },
    { ".bat", "" },      { ".mak", "" },   { ".desktop", "" }, { ".conf", "" },
    { ".html", "" },     { ".htm", "" },   { ".xhtml", "" },   { ".md", "" },
    { ".markdown", "" }, { ".xml", "󰗀" },  { ".xaml", "󰙳" },   { ".svg", "󰜡" },
    { ".tex", "" },      { ".wxs", "" },   { ".csproj", "" },  { ".wixproj", "" },
    { ".json", "" },     { ".1", "" },     { ".lock", "" },    { ".log", "" },
    { ".py", "" },       { ".css", "" },   { ".scss", "" },    { ".sass", "" },
    { ".less", "" },     { ".styl", "" },  { ".stylus", "" },  { ".pcss", "" },
    { ".vim", "" },      { ".lua", "" },   { ".cmd", "" },     { ".jsx", "" },
    { ".tsx", "" },
};

inline const std::unordered_map<std::string, std::string> directory_icon_map = {
    { ".git", "" },     { "include", "󱁿" },   { "src", "󰣞" },    { "node_modules", "" },
    { ".config", "󱁿" }, { ".vscode", "󰨞" },   { ".idea", "" },   { "cmake", "" },
    { "scripts", "󰴉" }, { "docs", "󱂷" },      { "doc", "󱂷" },    { "assets", "󰉏" },
    { "res", "󰉏" },     { "resources", "󰉏" }, { "public", "󰙉" }, { "test", "󰙨" },
    { "tests", "󰙨" },   { "__tests__", "󰙨" }, { "debug", "󰃤" },  { "benchmark", "󰅒" },
    { "api", "󱂛" },     { "client", "󱩛" },    { "server", "" },  { ".github", "" }
};

[[nodiscard]] std::string
get_item_icon(const std::filesystem::path& path);

} // namespace helpers

} // namespace ls

} // namespace fima
