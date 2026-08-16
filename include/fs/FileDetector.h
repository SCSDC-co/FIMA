/*
 * src/fs/FileDetector.cpp
 * include/fs/FileDetector.h
 *
 * A RAII wrapper around libmagic
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <cstdlib>
#include <magic.h>
#include <stdexcept>

namespace fima {

namespace fs {

class FileDetector
{
  public:
#ifndef _WIN32
#  ifndef FIMA_MAGIC_DATABASE
#    error "FIMA_MAGIC_DATABASE is not defined"
#  endif
    FileDetector()
      : magic(magic_open(MAGIC_MIME_TYPE | MAGIC_SYMLINK))
    {
        if (this->magic == nullptr) {
            throw std::runtime_error("Failed to initialize libmagic");
        }

        const char* magic_file = std::getenv("MAGIC");

        if (magic_load(this->magic, magic_file ? magic_file : FIMA_MAGIC_DATABASE) != 0) {
            throw std::runtime_error(magic_error(this->magic));
        }
    }

    ~FileDetector()
    {
        if (this->magic != nullptr) {
            magic_close(this->magic);
        }
    }
#endif

  private:
#ifndef _WIN32
    magic_t magic{};
#endif
};

inline FileDetector file_detector{};

} // namespace fs

} // namespace fima
