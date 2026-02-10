#include "commands/cloc/cloc.h"

#include <iostream>
#include <nlohmann/json.hpp>

#include "commands/cloc/helpers/language_file.h"

namespace fima {

namespace cloc {

using json = nlohmann::json;

void
main()
{
    json languages_file = helpers::get_languages_file();

    std::cout << languages_file.dump(2, ' ', true) << '\n';
}

}

}
