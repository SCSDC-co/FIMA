#include "commands/cloc/cloc.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace fima {

namespace cloc {

using json = nlohmann::json;

void
main()
{
    std::string language = R"(
    {
        "languages": [
            {
                "name": "C++",
                "extensions": [
                    ".cpp",
                    ".cxx",
                    ".cc"
                ],
                "comments": {
                    "single": "//",
                    "multiline_start": "/*",
                    "multiline_end": "*/"
                }
            }
        ]
    }
    )";

    json j = json::parse(language);

    std::vector<std::string> language_extensions;

    std::string language_name;
    std::string language_comment_single;
    std::string language_comment_multiline_start;
    std::string language_comment_multiline_end;

    for (const json& lang : j["languages"]) {
        language_name = lang["name"].get<std::string>();
        language_extensions = lang["extensions"].get<std::vector<std::string>>();
        language_comment_single = lang["comments"]["single"].get<std::string>();
        language_comment_multiline_start = lang["comments"]["multiline_start"].get<std::string>();
        language_comment_multiline_end = lang["comments"]["multiline_end"].get<std::string>();
    }

    std::cout << "language: " << language_name << '\n';
    std::cout << "extensions:" << '\n';

    for (const std::string& item : language_extensions) {
        std::cout << "  " << item << '\n';
    }

    std::cout << "comments:" << '\n';
    std::cout << "  single: " << language_comment_single << '\n';
    std::cout << "  multi line start: " << language_comment_multiline_start << '\n';
    std::cout << "  multi line end: " << language_comment_multiline_end << '\n';
}

}

}
