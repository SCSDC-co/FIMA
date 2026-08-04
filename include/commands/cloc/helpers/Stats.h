/*
 * src/commands/cloc/helpers/Stats.cpp
 * include/commands/cloc/helpers/Stats.h
 *
 * A class that contains just stats
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

namespace fima {

namespace cloc {

namespace classes {

class Stats
{
  private:
    int code{ 0 };
    int blank_lines{ 0 };
    int comments{ 0 };
    int total{ 0 };

  public:
    Stats(const int& _code = 0, const int& _blank_lines = 0, const int& _comments = 0)
      : code(_code)
      , blank_lines(_blank_lines)
      , comments(_comments)
      , total(_code + _blank_lines + _comments) {};

    Stats operator+(const Stats& s)
    {
        return Stats(this->code + s.get_code(),
                     this->blank_lines + s.get_blank_lines(),
                     this->comments + s.get_comments());
    }

    Stats operator+=(const Stats& s)
    {
        this->code += s.code;
        this->blank_lines += s.blank_lines;
        this->comments += s.comments;

        this->set_total();

        return *this;
    }

    void set_code(const int& lines) { this->code = lines; }
    void set_blank_lines(const int& lines) { this->blank_lines = lines; }
    void set_comments(const int& lines) { this->comments = lines; }
    void set_total() { this->total = this->blank_lines + this->comments + this->code; }

    [[nodiscard]] int get_code() const { return this->code; }
    [[nodiscard]] int get_blank_lines() const { return this->blank_lines; }
    [[nodiscard]] int get_comments() const { return this->comments; }
    [[nodiscard]] int get_total() const { return this->total; }
};

} // namespace classes

} // namespace cloc

} // namespace fima
