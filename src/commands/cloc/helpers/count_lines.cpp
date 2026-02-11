#include "commands/cloc/helpers/count_lines.h"

#include <filesystem>
#include <fstream>
#include <string>

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
count_lines(const fs::path& file_path)
{
    FileStats file_stats;

    std::ifstream file(file_path);

    if (file.is_open()) {
        std::string line;

        int code_lines{ 0 };

        while (std::getline(file, line)) {
            ++code_lines;
        }

        file_stats.set_code(code_lines);
    }

    file_stats.set_total();

    return file_stats;
}

}

}

}
