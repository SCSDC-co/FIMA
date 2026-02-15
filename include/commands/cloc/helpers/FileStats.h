/*
 * src/commands/cloc/helpers/FileStats.cpp
 * include/commands/cloc/helpers/FileStats.h
 *
 * Declaration of the FileStats class
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

namespace fima {

namespace cloc {

namespace classes {

class FileStats
{
  private:
    int total{ 0 };
    int comment{ 0 };
    int blank{ 0 };
    int code{ 0 };

  public:
    void set_comment(const int& lines);
    void set_blank(const int& lines);
    void set_code(const int& lines);
    void set_total();

    [[nodiscard]] int get_comment() const;
    [[nodiscard]] int get_blank() const;
    [[nodiscard]] int get_code() const;
    [[nodiscard]] int get_total() const;
};

}

}

}
