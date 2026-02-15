/*
 * src/helpers/logger.cpp
 * include/helpers/logger.h
 *
 * This is the declaration of the logger of FIMA
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 */

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
log(logger_type type, const std::string& message, const std::string& additional_message);

}

}
