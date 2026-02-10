#include <filesystem>
#include <nlohmann/json.hpp>

namespace fima {

namespace cloc {

namespace helpers {

void
create_languages_file(std::filesystem::path path);

[[nodiscard]] nlohmann::json
get_languages_file();

}

}

}
