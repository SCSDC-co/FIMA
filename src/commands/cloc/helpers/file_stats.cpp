#include "../../../../include/commands/cloc/helpers/file_stats.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../../../include/helpers/get_directories_entries.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

LanguageSpec
FileStats::create_c_like() const
{
    LanguageSpec spec;

    //  FileType(name, extensions)
    spec.file_types = {
        FileType("C++", { ".cpp", ".cxx", ".cc", ".c++", ".C" }),
        FileType("C++ Header", { ".hpp", ".hh", ".hxx", ".inl", ".tpp" }),
        FileType("C", { ".c" }),
        FileType("C/C++ Header", { ".h" }),
        FileType("Rust", { ".rs" }),
        FileType("C#", { ".cs" }),
        FileType("Java", { ".java" }),
        FileType("Kotlin", { ".kt", ".kts" }),
        FileType("Go", { ".go" }),
        FileType("JavaScript", { ".js" }),
        FileType("React JavaScript", { ".jsx" }),
        FileType("TypeScript", { ".ts" }),
        FileType("React TypeScript", { ".tsx" }),
        FileType("JSONC", { ".jsonc" }),
        FileType("Groovy", { ".groovy" }),
        FileType("Swift", { ".swift" }),
        FileType("Objective-C", { ".m" }),
        FileType("Objective-C++", { ".mm" }),
        FileType("D", { ".d" }),
        FileType("Zig", { ".zig" }),
        FileType("Scala", { ".scala" }),
        FileType("GLSL", { ".vert", ".frag", ".glsl" }),
        FileType("HLSL", { ".hlsl" }),
    };

    spec.line_comments = { LineComment("//") };

    spec.block_comments = { BlockComment("/*", "*/") };

    if (!spec.is_valid()) {
        throw std::runtime_error("The object is not valid");
    }

    return spec;
}

LanguageSpec
FileStats::create_shell_like() const
{
    LanguageSpec spec;

    //  FileType(name, extensions)
    //  Why the fuck clang-format is formatting like that
    spec.file_types = { FileType("Bash", { ".sh", ".bash" }),
                        FileType("ZSH", { ".zsh" }),
                        FileType("Python", { ".py" }),
                        FileType("CMake", { ".cmake" }, { "CMakeLists.txt" }),
                        FileType("Makefile", { "" }, { "Makefile" }),
                        FileType("YAML", { ".yaml", ".yml" }),
                        FileType("INI", { ".ini" }),
                        FileType("Config", { ".config" }),
                        FileType("TOML", { ".toml" }) };

    spec.line_comments = { LineComment("#") };

    if (!spec.is_valid()) {
        throw std::runtime_error("The object is not valid");
    }

    return spec;
}

LanguageSpec
FileStats::create_lisp_like() const
{
    LanguageSpec spec;

    // FileType(name, extensions)
    // I fucking hate this
    spec.file_types = { FileType("Lisp", { ".lisp", ".cl", ".lsp" }),
                        FileType("Scheme", { ".scm", ".ss" }),
                        FileType("Emacs Lisp", { ".el" }),
                        FileType("Racket", { ".rkt" }),
                        FileType("Hy", { ".hy" }),
                        FileType("Clojure", { ".clj", ".cljs", ".cljc" }) };

    spec.line_comments = { LineComment(";") };

    if (!spec.is_valid()) {
        throw std::runtime_error("The object is not valid");
    }

    return spec;
}

LanguageSpec
FileStats::create_asm_like() const
{
    LanguageSpec spec;

    spec.file_types = { FileType("Assembly", { ".asm", ".s", ".S", ".inc" }) };

    spec.line_comments = { LineComment(";") };

    if (!spec.is_valid()) {
        throw std::runtime_error("The object is not valid");
    }

    return spec;
}

LanguageSpec
FileStats::create_markup_like() const
{
    LanguageSpec spec;

    spec.file_types = { FileType("HTML", { ".html", ".html" }),
                        FileType("XML", { ".xml" }),
                        FileType("XHTML", { "xhtml" }),
                        FileType("SVG", { ".svg" }),
                        FileType("MathML", { ".mml" }),
                        FileType("Markdown", { ".md", ".markdown" }),
                        FileType("LaTeX", { ".tex" }),
                        FileType("reStructuredText", { ".rst" }),
                        FileType("AsciiDoc", { ".adoc", ".asciidoc" }),
                        FileType("PHP", { ".php" }) };

    spec.block_comments = { BlockComment("<!--", "-->") };

    if (!spec.is_valid()) {
        throw std::runtime_error("The object is not valid");
    }

    return spec;
}

LanguageSpec
FileStats::create_plain_text() const
{
    LanguageSpec spec;

    spec.file_types = { FileType("Plain Text", { ".txt" }),
                        FileType("CSV", { ".csv" }),
                        FileType("JSON", { ".json" }) };

    if (!spec.is_valid()) {
        throw std::runtime_error("The object is not valid");
    }

    return spec;
}

LanguageSpec
LanguageSpec::create(Type type)
{
    FileStats stats;

    switch (type) {
        case Type::CLike:
            return stats.create_c_like();
        case Type::ShellLike:
            return stats.create_shell_like();
        case Type::LispLike:
            return stats.create_lisp_like();
        case Type::Markup:
            return stats.create_markup_like();
        case Type::Asm:
            return stats.create_asm_like();
        case Type::PlainText:
            return stats.create_plain_text();
        default:
            return {};
    }
}

void
FileStats::count_lines(const fs::path& path) const
{
    std::string line;
    std::ifstream file(path);

    while (std::getline(file, line)) {
    }
}

std::vector<std::string>
FileStats::create_row(const std::string& language) const
{
    std::vector<std::string> row;

    return row;
}

void
FileStats::process(const fs::directory_entry& path) const
{
}

} // namespace cloc

} // namespace fima
