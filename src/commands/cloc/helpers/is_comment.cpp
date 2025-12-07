#include "../../../../include/commands/cloc/helpers/is_comment.h"

#include <regex>
#include <string>

#include "../../../../include/commands/cloc/helpers/regex.h"

bool is_comment(std::string line) {
    bool result{false};
    bool multi_line{false};

    result =
        std::regex_match(line, std::regex(fima::regex::SINGLE_LINE_COMMENT));

    return result;
}
