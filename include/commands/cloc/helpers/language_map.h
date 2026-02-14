#pragma once

#include <filesystem>
#include <string>

namespace fima {

namespace cloc {

namespace helpers {

[[nodiscard]] std::string
get_language_family(std::filesystem::path path);

[[nodiscard]] std::string
get_language_name(std::filesystem::path path);

}

}

}
