#pragma once

#include <filesystem>
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

class FileStats
{
  public:
    LanguageSpec create_c_like() const;
    LanguageSpec create_shell_like() const;
    LanguageSpec create_lisp_like() const;
    LanguageSpec create_asm_like() const;
    LanguageSpec create_markup_like() const;
    LanguageSpec create_plain_text() const;

    [[nodiscard]] bool is_comment(std::string line) const;
    [[nodiscard]] std::vector<std::string> create_row(
      const std::string& language) const;

    void count_lines(const fs::path& path) const;
    void process(const fs::directory_entry& path) const;
};

} // namespace cloc

} // namespace fima
