#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

class Validatable
{
  public:
    virtual ~Validatable() = default;

    [[nodiscard]] virtual bool is_valid() const = 0;
};

class LineComment : public Validatable
{
  public:
    std::string prefix; // required
    [[nodiscard]] bool is_valid() const override
    {
        return !this->prefix.empty();
    }

    LineComment(std::string pr)
      : prefix(std::move(pr))
    {
    }
};

class BlockComment : public Validatable
{
  public:
    std::string open;  // required
    std::string close; // required

    [[nodiscard]] bool is_valid() const override
    {
        return !this->open.empty() && !this->close.empty();
    };

    BlockComment(std::string op, std::string cl)
      : open(std::move(op))
      , close(std::move(cl))
    {
    }
};

class FileType : public Validatable
{
  public:
    std::string type_name; // required

    // at least 1 extension or exact_filename required
    std::vector<std::string> extensions;
    std::string exact_filename;

    [[nodiscard]] bool is_valid() const override
    {
        return !this->type_name.empty() &&
               (!this->extensions.empty() || !this->exact_filename.empty());
    }

    FileType(std::string tn,
             std::vector<std::string> exts = {},
             std::string exact = "")
      : type_name(std::move(tn))
      , extensions(std::move(exts))
      , exact_filename(std::move(exact))
    {
    }
};

class LanguageSpec : public Validatable
{
  public:
    enum class Type
    {
        CLike,
        ShellLike,
        LispLike,
        Markup,
        Asm,
        PlainText,
        Unkown,
        COUNT_
    };

    std::vector<FileType> file_types;
    std::vector<LineComment> line_comments;
    std::vector<BlockComment> block_comments;

    [[nodiscard]] bool is_valid() const override
    {
        return !this->file_types.empty();
    };

    [[nodiscard]] static LanguageSpec create(Type type);
};

class FileLines
{
  private:
    int total_lines;
    int blank_lines;
    int comment_lines;
    int code_lines;

  public:
    [[nodiscard]] int get_total_lines() const { return this->total_lines; }
    [[nodiscard]] int get_code_lines() const { return this->code_lines; }
    [[nodiscard]] int get_comment_lines() const { return this->comment_lines; }
    [[nodiscard]] int get_blank_lines() const { return this->blank_lines; }

    void set_code_lines(int lines) { this->code_lines = lines; }
    void set_comment_lines(int lines) { this->comment_lines = lines; }
    void set_blank_lines(int lines) { this->blank_lines = lines; }
    void set_total_lines()
    {
        this->total_lines =
          this->code_lines + this->comment_lines + this->blank_lines;
    }
};

class FileStats
{
  public:
    LanguageSpec create_c_like() const;
    LanguageSpec create_shell_like() const;
    LanguageSpec create_lisp_like() const;
    LanguageSpec create_asm_like() const;
    LanguageSpec create_markup_like() const;
    LanguageSpec create_plain_text() const;

    const std::unordered_map<std::string, LanguageSpec::Type>
      EXTENSION_TYPE_MAP = {
          // --- C-LIKE ---
          { ".cpp", LanguageSpec::Type::CLike },
          { ".cxx", LanguageSpec::Type::CLike },
          { ".cc", LanguageSpec::Type::CLike },
          { ".c++", LanguageSpec::Type::CLike },
          { ".C", LanguageSpec::Type::CLike },
          { ".hpp", LanguageSpec::Type::CLike },
          { ".hh", LanguageSpec::Type::CLike },
          { ".hxx", LanguageSpec::Type::CLike },
          { ".inl", LanguageSpec::Type::CLike },
          { ".tpp", LanguageSpec::Type::CLike },
          { ".c", LanguageSpec::Type::CLike },
          { ".h", LanguageSpec::Type::CLike },
          { ".rs", LanguageSpec::Type::CLike },
          { ".cs", LanguageSpec::Type::CLike },
          { ".java", LanguageSpec::Type::CLike },
          { ".kt", LanguageSpec::Type::CLike },
          { ".kts", LanguageSpec::Type::CLike },
          { ".go", LanguageSpec::Type::CLike },
          { ".js", LanguageSpec::Type::CLike },
          { ".jsx", LanguageSpec::Type::CLike },
          { ".ts", LanguageSpec::Type::CLike },
          { ".tsx", LanguageSpec::Type::CLike },
          { ".jsonc", LanguageSpec::Type::CLike },
          { ".groovy", LanguageSpec::Type::CLike },
          { ".swift", LanguageSpec::Type::CLike },
          { ".m", LanguageSpec::Type::CLike },
          { ".mm", LanguageSpec::Type::CLike },
          { ".d", LanguageSpec::Type::CLike },
          { ".zig", LanguageSpec::Type::CLike },
          { ".scala", LanguageSpec::Type::CLike },
          { ".vert", LanguageSpec::Type::CLike },
          { ".frag", LanguageSpec::Type::CLike },
          { ".glsl", LanguageSpec::Type::CLike },
          { ".hlsl", LanguageSpec::Type::CLike },

          // --- SHELL-LIKE ---
          { ".sh", LanguageSpec::Type::ShellLike },
          { ".bash", LanguageSpec::Type::ShellLike },
          { ".zsh", LanguageSpec::Type::ShellLike },
          { ".py", LanguageSpec::Type::ShellLike },
          { ".cmake", LanguageSpec::Type::ShellLike },
          { ".yaml", LanguageSpec::Type::ShellLike },
          { ".yml", LanguageSpec::Type::ShellLike },
          { ".ini", LanguageSpec::Type::ShellLike },
          { ".config", LanguageSpec::Type::ShellLike },
          { ".toml", LanguageSpec::Type::ShellLike },

          // --- LISP-LIKE ---
          { ".lisp", LanguageSpec::Type::LispLike },
          { ".cl", LanguageSpec::Type::LispLike },
          { ".lsp", LanguageSpec::Type::LispLike },
          { ".scm", LanguageSpec::Type::LispLike },
          { ".ss", LanguageSpec::Type::LispLike },
          { ".el", LanguageSpec::Type::LispLike },
          { ".rkt", LanguageSpec::Type::LispLike },
          { ".hy", LanguageSpec::Type::LispLike },
          { ".clj", LanguageSpec::Type::LispLike },
          { ".cljs", LanguageSpec::Type::LispLike },
          { ".cljc", LanguageSpec::Type::LispLike },

          // --- ASM ---
          { ".asm", LanguageSpec::Type::Asm },
          { ".s", LanguageSpec::Type::Asm },
          { ".S", LanguageSpec::Type::Asm },
          { ".inc", LanguageSpec::Type::Asm },

          // --- MARKUP ---
          { ".html", LanguageSpec::Type::Markup },
          { ".xml", LanguageSpec::Type::Markup },
          { ".xhtml", LanguageSpec::Type::Markup },
          { ".svg", LanguageSpec::Type::Markup },
          { ".mml", LanguageSpec::Type::Markup },
          { ".md", LanguageSpec::Type::Markup },
          { ".markdown", LanguageSpec::Type::Markup },
          { ".tex", LanguageSpec::Type::Markup },
          { ".rst", LanguageSpec::Type::Markup },
          { ".adoc", LanguageSpec::Type::Markup },
          { ".asciidoc", LanguageSpec::Type::Markup },
          { ".php", LanguageSpec::Type::Markup },

          // --- PLAIN TEXT ---
          { ".txt", LanguageSpec::Type::PlainText },
          { ".csv", LanguageSpec::Type::PlainText },
          { ".json", LanguageSpec::Type::PlainText },
      };
    // there are 75 total extensions, WTF

    [[nodiscard]] std::vector<std::string> create_row(
      const std::string& language) const;

    [[nodiscard]] bool is_blank(const std::string& s) const
    {
        return std::all_of(
          s.begin(), s.end(), [](unsigned char c) { return std::isspace(c); });
    }

    [[nodiscard]] FileLines count_lines(const fs::path& path,
                                        LanguageSpec::Type type) const;
    void process(const fs::directory_entry& path) const;
};

} // namespace cloc

} // namespace fima
