#include "commands/copy/directory.h"

#include <filesystem>
#include <iostream>

#include "helpers/colors.h"
#include "helpers/logger.h"

namespace fs = std::filesystem;

namespace fima {

namespace copy {

void
directory(fs::path source, fs::path destination)
{
    if (!fs::is_directory(source)) {
        fima::helpers::log(fima::helpers::logger_type::ERROR,
                           "The source file is a file or does not exists: ",
                           source.string());
    }

    if (!fs::is_directory(destination)) {
        fs::create_directory(destination);
    }

    try {
        fs::copy(source,
                 destination,
                 fs::copy_options::overwrite_existing |
                   fs::copy_options::recursive);

        std::clog << fima::colors::GREEN << "Directory " << fima::colors::RESET
                  << source.string() << fima::colors::GREEN << " copied to "
                  << fima::colors::RESET << destination.string() << '\n';
    } catch (const std::exception& ex) {
        fima::helpers::log(fima::helpers::logger_type::ERROR,
                           "Failed to copy the directory: ",
                           "");

        fima::helpers::log(
          fima::helpers::logger_type::ERROR, "  Source directory: ", source);
        fima::helpers::log(
          fima::helpers::logger_type::ERROR, "  Destination: ", destination);

        fima::helpers::log(fima::helpers::logger_type::ERROR, "", ex.what());
    }
}

} // namespace copy

} // namespace fima
