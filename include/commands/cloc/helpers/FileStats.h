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
