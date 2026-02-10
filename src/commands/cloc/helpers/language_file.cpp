#include "commands/cloc/helpers/language_file.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "helpers/get_data_path.h"

namespace fs = std::filesystem;

namespace fima {

namespace cloc {

namespace helpers {

using json = nlohmann::json;

void
create_languages_file(fs::path path)
{
    // if the file exists we exit since it doesn't need to be created
    if (fs::exists(path)) {
        return;
    }

    std::string languages_file = R"(
    {
        "c_like": {
            "comments": {
                "single": "//",
                "multiline_start": "/*",
                "multiline_end": "*/"
            }
        },
        "lisp_like": {
            "comments": {
                "single": ";",
                "multiline_start": null,
                "multiline_end": null
            }
        },
        "shell_like": {
            "comments": {
                "single": "#",
                "multiline_start": null,
                "multiline_end": null
            }
        },
        "python": {
            "comments": {
                "single": "#",
                "multiline_start": "\"\"\"",
                "multiline_end": "\"\"\""
            }
        }
    }
    )";

    json json_file = json::parse(languages_file);

    static std::ofstream file_path;

    file_path.open(path, std::ios::app);
    file_path << json_file.dump(2, ' ', true);
    file_path.close();
}

[[nodiscard]] json
get_languages_file()
{
    json file;

    fs::path data_path          = fima::helpers::get_application_data_path();
    fs::path fima_data_path     = data_path / "fima";
    fs::path language_file_path = fima_data_path / "languages.json";

    if (!fs::exists(fima_data_path)) {
        fs::create_directory(fima_data_path);
    }

    create_languages_file(language_file_path);

    std::ifstream file_stream(language_file_path);
    file = json::parse(file_stream);

    return file;
}

}

}

}
