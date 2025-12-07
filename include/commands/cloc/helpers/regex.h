#pragma once

namespace fima {

namespace regex {

constexpr auto SINGLE_LINE_COMMENT =
    "^ *(//|;|#|--|/\\*.*\\*/"
    "|<!--.*-->|\"\"\".*\"\"\"|'''.*'''|\\\{-.*-}|--\\\[\\\[.*\\]\\]).*$";

constexpr auto MULTI_LINE_COMMENT_START =
    "^ *(/\\*.*|<!--.*|\\\{-.*|\"\"\".*|'''.*|--\\\[\\\[.*).*$";

constexpr auto MULTI_LINE_COMMENT_END = "^ *(\\*/|-->|-}|\"\"\"|'''|\\]\\]).*$";

} // namespace regex

} // namespace fima
