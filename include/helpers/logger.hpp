#pragma once

#include <iostream>

#include "helpers/colors.h"

namespace fima {

namespace helpers {

enum class logger_type
{
    LOG,
    ERROR
};

inline void
log(logger_type type,
    const std::string& message,
    const std::string& additional_message)
{
    switch (type) {
        case logger_type::LOG:
            std::clog << fima::colors::GREEN << message << fima::colors::RESET
                      << additional_message << '\n';
            break;

        case logger_type::ERROR:
            std::cerr << fima::colors::RED << message << fima::colors::RESET
                      << additional_message << std::endl;
            break;
    }
}

}

}
