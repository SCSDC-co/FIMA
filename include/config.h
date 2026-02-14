#pragma once

#include <string>
#include <string_view>
#include <unordered_set>

namespace fima {

namespace config {

constexpr std::string_view VERSION = "1.0.0";
constexpr std::string_view LOGO    = R"(
.----.______       _____ ___ __  __    _
|           |     |  ___|_ _|  \/  |  / \
|   ___________   | |_   | || |\/| | / _ \
|  /          /   |  _|  | || |  | |/ ___ \
| /          /    |_|   |___|_|  |_/_/   \_\
|/__________/
)";

// since these are useless to analyze we put them as default to ignore
inline static const std::unordered_set<std::string> DEFAULT_DIRS_TO_IGNORE = {
    ".git", ".cache", ".vscode", "build", "dist", "target", "node_modules", ".next"
};

}

}
