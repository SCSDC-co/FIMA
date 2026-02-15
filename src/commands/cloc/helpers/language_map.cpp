#include "commands/cloc/helpers/language_map.h"

#include <filesystem>
#include <string>
#include <unordered_set>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

namespace helpers {

[[nodiscard]] std::string
get_language_family(fs::path path)
{
    std::string filename = path.filename().string();

    static const std::unordered_set<std::string> special_shell_type = {
        "CMakeLists.txt", ".gitignore",  ".clangd", ".clang-format", ".editorconfig",
        "Makefile",       "BSDmakefile", ".bashrc", ".zshrc",        ".styluaignore"
    };

    static const std::unordered_set<std::string> special_text_type = { "LICENSE",
                                                                       "license",
                                                                       ".mailmap" };

    static const std::unordered_set<std::string> special_lua_type = {
        ".luacov",
        ".luacheckrc",
    };

    if (special_text_type.contains(filename)) {
        return "text";
    }

    if (special_shell_type.contains(filename)) {
        return "shell_like";
    }

    if (special_lua_type.contains(filename)) {
        return "lua";
    }

    std::string extension = path.extension();

    auto language = language_map_family.find(extension);

    if (language == language_map_family.end()) {
        return "text";
    }

    return language_map_family.at(extension);
}

[[nodiscard]] std::string
get_language_name(fs::path path)
{
    std::string filename = path.filename().string();

    static const std::unordered_set<std::string> special_shell = {
        ".gitignore",     ".clangd",    ".clang-format", ".editorconfig", ".git-blame-ignore-revs",
        ".gitattributes", ".clang-tidy"
    };

    if (filename == "CMakeLists.txt") {
        return "CMake";
    } else if (filename == "LICENSE" || filename == "license") {
        return "LICENSE";
    } else if (filename == "Makefile" || filename == "BSDmakefile") {
        return "Make";
    } else if (special_shell.contains(filename)) {
        return "YAML";
    }

    std::string extension = path.extension();

    auto language = language_map_name.find(extension);

    if (language == language_map_name.end()) {
        return "Text";
    }

    return language_map_name.at(extension);
}

}

}

}
