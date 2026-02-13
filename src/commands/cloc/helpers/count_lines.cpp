#include "commands/cloc/helpers/count_lines.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "helpers/trim_string.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

namespace helpers {

void
FileStats::set_comment(const int& lines)
{
    this->comment = lines;
}
void
FileStats::set_blank(const int& lines)
{
    this->blank = lines;
}
void
FileStats::set_code(const int& lines)
{
    this->code = lines;
}
void
FileStats::set_total()
{
    this->total = this->blank + this->comment + this->code;
}

[[nodiscard]] int
FileStats::get_comment() const
{
    return this->comment;
}
[[nodiscard]] int
FileStats::get_blank() const
{
    return this->blank;
}
[[nodiscard]] int
FileStats::get_code() const
{
    return this->code;
}
[[nodiscard]] int
FileStats::get_total() const
{
    return this->total;
}

FileStats
count_lines(const fs::path& file_path,
            const std::string& single_comment,
            const std::string& multiline_start,
            const std::string& multiline_end)
{
    FileStats file_stats;

    std::ifstream file(file_path);

    if (file.is_open()) {
        std::string line;

        int code_lines{ 0 };
        int blank_lines{ 0 };
        int comment_lines{ 0 };

        bool can_have_single_comments{ !single_comment.empty() };
        bool can_have_multiline_comments{ !(multiline_start.empty() || multiline_end.empty()) };
        bool is_multiline{ false };

        while (std::getline(file, line)) {
            std::string trimmed_line = fima::helpers::trim(line);

            if (trimmed_line.empty()) {
                ++blank_lines;

                continue;
            }

            if (can_have_single_comments && trimmed_line.starts_with(single_comment)) {
                ++comment_lines;

                continue;
            }

            if (can_have_multiline_comments) {
                if (trimmed_line.starts_with(multiline_start) &&
                    trimmed_line.ends_with(multiline_end)) {
                    ++comment_lines;

                    continue;
                }

                if (is_multiline && !trimmed_line.ends_with(multiline_end)) {
                    ++comment_lines;

                    continue;
                }

                if (!is_multiline && trimmed_line.starts_with(multiline_start)) {
                    ++comment_lines;

                    is_multiline = true;

                    continue;
                }

                if (is_multiline && trimmed_line.starts_with(multiline_end)) {
                    is_multiline = false;

                    ++comment_lines;

                    continue;
                }
            }

            ++code_lines;
        }

        file_stats.set_code(code_lines);
        file_stats.set_blank(blank_lines);
        file_stats.set_comment(comment_lines);
    }

    file_stats.set_total();

    return file_stats;
}

}

}

}
