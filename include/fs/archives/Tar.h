/*
 * scr/fs/archives/Tar.cpp
 * include/fs/archives/Tar.h
 *
 * A utility for working with tar archives.
 * To keep the codebase fully in C++, we implemented a custom tar archive generator.
 *
 * Parts of this implementation are derived from:
 * https://github.com/lindenb/cclindenb/
 * Thanks guys ❤
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <array>

namespace fima {

namespace fs {

namespace archives {

class Tar
{
  public:
  private:
    struct Header
    {
        std::array<char, 100> name;
        std::array<char, 8> mode;
        std::array<char, 8> uid;
        std::array<char, 8> gid;
        std::array<char, 12> size;
        std::array<char, 12> mtime;
        std::array<char, 8> checksum;
        std::array<char, 1> typeflag;
        std::array<char, 100> linkname;
        std::array<char, 6> magic;
        std::array<char, 2> version;
        std::array<char, 32> uname;
        std::array<char, 32> gname;
        std::array<char, 8> devmajor;
        std::array<char, 8> devminor;
        std::array<char, 155> prefix;
        std::array<char, 12> pad;
    };

    static_assert(sizeof(Header) == 512, "The tar header must be 512 bytes");
};

} // namespace archives

} // namespace fs

} // namespace fima
