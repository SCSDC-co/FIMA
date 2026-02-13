#include "commands/cloc/helpers/FileStats.h"

namespace fima {

namespace cloc {

namespace classes {

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

}

}

}
