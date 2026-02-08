#include "commands/remove.h"

#include <filesystem>
#include <vector>

#include "helpers/logger.hpp"

namespace fs = std::filesystem;

namespace fima {

void
remove(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {
        if (!fs::exists(entry)) {
            fima::helpers::log(fima::helpers::logger_type::ERROR,
                               "The path doesn't exists: ",
                               entry.string());

            continue;
        }

        try {
            fs::remove_all(entry);

            fima::helpers::log(fima::helpers::logger_type::LOG,
                               "Item removed: ",
                               entry.string());
        } catch (const std::exception& ex) {
            fima::helpers::log(fima::helpers::logger_type::ERROR,
                               "Failed to remove directory: ",
                               entry.string());
            fima::helpers::log(
              fima::helpers::logger_type::ERROR, "", ex.what());
        }
    }
}

} // namespace fima
