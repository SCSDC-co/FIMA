#pragma once

#include <string_view>

namespace fima {

namespace colors {

inline constexpr std::string_view RESET = "\033[0m";

inline constexpr std::string_view GREEN = "\033[32m";
inline constexpr std::string_view RED = "\033[31m";
inline constexpr std::string_view BLUE = "\033[34m";
inline constexpr std::string_view YELLOW = "\033[33m";

inline constexpr std::string_view BOLD = "\033[1m";

}

}
