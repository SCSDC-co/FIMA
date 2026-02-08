#include "commands/create/file.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "helpers/logger.hpp"

namespace fs = std::filesystem;

namespace fima {

namespace create {

void
file(const std::vector<fs::path>& paths)
{
    for (const auto& entry : paths) {
        if (fs::is_regular_file(entry)) {
            fima::helpers::log(fima::helpers::logger_type::ERROR,
                               "This file already exists: ",
                               entry.string());

            continue;
        }

        try {
            std::ofstream outfile{ entry };
            outfile.close();

            fima::helpers::log(fima::helpers::logger_type::LOG,
                               "File created at: ",
                               entry.string());
        } catch (const std::exception& ex) {
            fima::helpers::log(fima::helpers::logger_type::ERROR,
                               "Failed to create the file: ",
                               entry.string());
            fima::helpers::log(
              fima::helpers::logger_type::ERROR, "", ex.what());
        }
    }
}

} // namespace create

} // namespace fima
