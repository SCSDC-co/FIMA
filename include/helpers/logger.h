#pragma once

#include <string>

namespace fima {

namespace helpers {

enum class logger_type
{
    LOG,
    ERROR
};

void
log(logger_type type,
    const std::string& message,
    const std::string& additional_message);

}

}
