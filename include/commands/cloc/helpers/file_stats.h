#pragma once

#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

class FileStats {
  private:
    fs::path path;
    int code;
    int blank;
    int comment;
    int total;

    static constexpr auto SINGLE_LINE_COMMENT =
        "^ *(//|;|#|--|/\\*.*\\*/"
        "|<!--.*-->|\"\"\".*\"\"\"|'''.*'''|\\\{-.*-}|--\\\[\\\[.*\\]\\]).*$";
    static constexpr auto MULTI_LINE_COMMENT_START =
        "^ *(/\\*.*|<!--.*|\\\{-.*|\"\"\".*|'''.*|--\\\[\\\[.*).*$";
    static constexpr auto MULTI_LINE_COMMENT_END =
        "^ *(\\*/|-->|-}|\"\"\"|'''|\\]\\]).*$";

    bool is_comment(std::string line) {
        bool result{false};
        bool multi_line{false};

        result = std::regex_match(line, std::regex(this->SINGLE_LINE_COMMENT));

        return result;
    }

  public:
    void count_lines(const fs::path &path);
    void process(const fs::directory_entry &path);
    void print();
};

} // namespace cloc

} // namespace fima
