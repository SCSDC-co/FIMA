#include "commands/cloc/helpers/language_map.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

namespace helpers {

// this is the map for extension -> family
std::unordered_map<std::string, std::string> language_map_family = {
    // c-like
    { ".cpp", "c_like" },
    { ".cxx", "c_like" },
    { ".cc", "c_like" },
    { ".h", "c_like" },
    { ".hpp", "c_like" },
    { ".hxx", "c_like" },
    { ".inl", "c_like" },
    { ".rs", "c_like" },
    { ".cs", "c_like" },
    { ".js", "c_like" },
    { ".ts", "c_like" },
    { ".java", "c_like" },
    { ".m", "c_like" },
    { ".php", "c_like" },
    { ".dart", "c_like" },
    { ".go", "c_like" },
    { ".swift", "c_like" },
    { ".kt", "c_like" },
    { ".scala", "c_like" },
    { ".hx", "c_like" },
    { ".hxml", "c_like" },
    { ".zig", "c_like" },
    { ".jsonc", "c_like" },

    // lisp-like
    { ".lisp", "lisp_like" },
    { ".lsp", "lisp_like" },
    { ".cl", "lisp_like" },
    { ".scm", "lisp_like" },
    { ".rkt", "lisp_like" },
    { ".clj", "lisp_like" },
    { ".cljs", "lisp_like" },
    { ".cljc", "lisp_like" },
    { ".edn", "lisp_like" },
    { ".el", "lisp_like" },
    { ".ss", "lisp_like" },
    { ".hy", "lisp_like" },
    { ".janet", "lisp_like" },
    { ".fnl", "lisp_like" },
    { ".asm", "lisp_like" },
    { ".s", "lisp_like" },

    // shell-like
    { ".sh", "shell_like" },
    { ".zsh", "shell_like" },
    { ".fish", "shell_like" },
    { ".dash", "shell_like" },
    { ".ksh", "shell_like" },
    { ".ash", "shell_like" },
    { ".tcsh", "shell_like" },
    { ".csh", "shell_like" },
    { ".rb", "shell_like" },
    { ".pl", "shell_like" },
    { ".pm", "shell_like" },
    { ".raku", "shell_like" },
    { ".rakumod", "shell_like" },
    { ".p6", "shell_like" },
    { ".jl", "shell_like" },
    { ".nim", "shell_like" },
    { ".cr", "shell_like" },
    { ".tf", "shell_like" },
    { ".nix", "shell_like" },
    { ".sls", "shell_like" },
    { ".cmake", "shell_like" },
    { ".yaml", "shell_like" },
    { ".yml", "shell_like" },
    { ".toml", "shell_like" },
    { ".ini", "shell_like" },

    // markup
    { ".html", "markup" },
    { ".htm", "markup" },
    { ".xhtml", "markup" },
    { ".md", "markup" },
    { ".markdown", "markup" },
    { ".xml", "markup" },
    { ".xaml", "markup" },
    { ".svg", "markup" },
    { ".rst", "markup" },
    { ".tex", "markup" },
    { ".adoc", "markup" },

    // "special" languages
    { ".py", "python" },
    { ".txt", "text" },
    { ".json", "text" },
    { ".css", "css" },
    { ".scss", "css" },
    { ".sass", "css" },
    { ".less", "css" },
    { ".styl", "css" },
    { ".stylus", "css" },
    { ".pcss", "css" },
};

// this one is for extension -> language
std::unordered_map<std::string, std::string> language_map_language = {
    // c-like
    { ".cpp", "C++" },
    { ".cxx", "C++" },
    { ".cc", "C++" },
    { ".h", "C Header" },
    { ".hpp", "C++ Header" },
    { ".hxx", "C++ Header" },
    { ".inl", "C++ Header" },
    { ".rs", "Rust" },
    { ".cs", "C#" },
    { ".js", "JavaScript" },
    { ".ts", "TypeScript" },
    { ".java", "Java" },
    { ".m", "Objective-C" },
    { ".php", "PHP" },
    { ".dart", "Dart" },
    { ".go", "Go" },
    { ".swift", "Swift" },
    { ".kt", "Kotlin" },
    { ".scala", "Scala" },
    { ".hx", "Haxe" },
    { ".hxml", "Haxe" },
    { ".zig", "Zig" },

    // lisp-like
    { ".lisp", "Common Lisp" },
    { ".lsp", "Common Lisp" },
    { ".cl", "Common Lisp" },
    { ".scm", "Scheme" },
    { ".rkt", "Racket" },
    { ".ss", "Racket" },
    { ".clj", "Clojure" },
    { ".cljs", "Clojure" },
    { ".cljc", "Clojure" },
    { ".edn", "EDN" },
    { ".el", "Emacs Lisp" },
    { ".hy", "Hy" },
    { ".janet", "Janet" },
    { ".fnl", "Fennel" },
    { ".asm", "Assembly" },
    { ".s", "Assembly" },

    // shell-like
    { ".sh", "Bash" },
    { ".zsh", "ZSH" },
    { ".fish", "Fish" },
    { ".dash", "Dash" },
    { ".ksh", "KSH" },
    { ".ash", "ASH" },
    { ".tcsh", "TSCH" },
    { ".csh", "CSH" },
    { ".rb", "Ruby" },
    { ".pl", "PL/I" },
    { ".pm", "PM" },
    { ".raku", "Raku" },
    { ".rakumod", "Raku" },
    { ".p6", "P6" },
    { ".jl", "Julia" },
    { ".nim", "Nim" },
    { ".cr", "Crystal" },
    { ".tf", "Tensor Flow" },
    { ".nix", "NIX" },
    { ".sls", "S/LS" },
    { ".cmake", "CMake" },
    { ".yaml", "YAML" },
    { ".yml", "YAML" },
    { ".toml", "TOML" },
    { ".ini", "INI" },

    // markup
    { ".html", "HTML" },
    { ".htm", "HTML" },
    { ".xhtml", "XHTML" },
    { ".md", "Markdown" },
    { ".markdown", "Markdown" },
    { ".xml", "XML" },
    { ".xaml", "XAML" },
    { ".svg", "SVG" },
    { ".rst", "RST" },
    { ".tex", "TeX" },
    { ".adoc", "ASCII Doc" },

    // "special" languages
    { ".py", "Python" },
    { ".txt", "Plain Text" },
    { ".css", "CSS" },
    { ".scss", "CSS" },
    { ".sass", "CSS" },
    { ".less", "CSS" },
    { ".styl", "CSS" },
    { ".stylus", "CSS" },
    { ".pcss", "CSS" },
    { ".json", "JSON" },
    { ".jsonc", "JSONC" },
};

[[nodiscard]] std::string
get_language_family(fs::path path)
{
    std::string filename = path.filename();

    if (filename == "LICENSE" || filename == "license") {
        return "text";
    }

    static const std::unordered_set<std::string> special_shell_type = {
        "CMakeLists.txt", ".gitignore", ".clangd", ".clang-format", ".editorconfig"
    };

    if (special_shell_type.contains(filename)) {
        return "shell_like";
    }

    std::string extension = path.extension();

    return language_map_family.at(extension);
}

[[nodiscard]] std::string
get_language_name(fs::path path)
{
    std::string filename = path.filename();

    static const std::unordered_set<std::string> special_yaml = {
        ".gitignore", ".clangd", ".clang-format", ".editorconfig"
    };

    if (filename == "CMakeLists.txt") {
        return "CMake";
    } else if (filename == "LICENSE" || filename == "license") {
        return "LICENSE";
    } else if (special_yaml.contains(filename)) {
        return "YAML";
    }

    std::string extension = path.extension();

    return language_map_language.at(extension);
}

}

}

}
