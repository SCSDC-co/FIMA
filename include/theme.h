/*
 * include/theme.h
 * src/theme.cpp
 *
 * Logic for the themes
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#pragma once

#include <array>
#include <cstdint>
#include <ftxui/screen/color.hpp>
#include <ostream>
#include <rang.hpp>
#include <string_view>
#include <unordered_map>

#ifdef _WIN32
#  include <windows.h>
#endif

namespace fima {

namespace theme {

enum class ColorType
{
    RGB,
    NAMED
};

enum class NamedColor
{
    Black,
    DarkGray,
    LightGray,
    White,
    Blue,
    LightBlue,
    Cyan,
    LightCyan,
    Green,
    LightGreen,
    Magenta,
    LightMagenta,
    Red,
    LightRed,
    Yellow,
    LightYellow
};

inline const std::unordered_map<char, std::uint8_t> char_to_number_hex{
    { '0', 0 },  { '1', 1 },  { '2', 2 },  { '3', 3 },  { '4', 4 },  { '5', 5 },
    { '6', 6 },  { '7', 7 },  { '8', 8 },  { '9', 9 },  { 'a', 10 }, { 'b', 11 },
    { 'c', 12 }, { 'd', 13 }, { 'e', 14 }, { 'f', 15 },
};

inline const std::unordered_map<std::string_view, NamedColor> name_to_named_color{
    { "black", NamedColor::Black },
    { "dark grey", NamedColor::DarkGray },
    { "light grey", NamedColor::LightGray },
    { "dark gray", NamedColor::DarkGray },
    { "light gray", NamedColor::LightGray },
    { "white", NamedColor::White },
    { "blue", NamedColor::Blue },
    { "light blue", NamedColor::LightBlue },
    { "cyan", NamedColor::Cyan },
    { "light cyan", NamedColor::LightCyan },
    { "green", NamedColor::Green },
    { "light green", NamedColor::LightGreen },
    { "magenta", NamedColor::Magenta },
    { "light magenta", NamedColor::LightMagenta },
    { "red", NamedColor::Red },
    { "light red", NamedColor::LightRed },
    { "yellow", NamedColor::Yellow },
    { "light yellow", NamedColor::LightYellow },
};

inline const std::unordered_map<NamedColor, ftxui::Color> named_to_ftxui_color{
    { NamedColor::Black, ftxui::Color::Black },
    { NamedColor::DarkGray, ftxui::Color::GrayDark },
    { NamedColor::LightGray, ftxui::Color::GrayLight },
    { NamedColor::White, ftxui::Color::White },
    { NamedColor::Blue, ftxui::Color::Blue },
    { NamedColor::LightBlue, ftxui::Color::BlueLight },
    { NamedColor::Cyan, ftxui::Color::Cyan },
    { NamedColor::LightCyan, ftxui::Color::CyanLight },
    { NamedColor::Green, ftxui::Color::Green },
    { NamedColor::LightGreen, ftxui::Color::GreenLight },
    { NamedColor::Magenta, ftxui::Color::Magenta },
    { NamedColor::LightMagenta, ftxui::Color::MagentaLight },
    { NamedColor::Red, ftxui::Color::Red },
    { NamedColor::LightRed, ftxui::Color::RedLight },
    { NamedColor::Yellow, ftxui::Color::Yellow },
    { NamedColor::LightYellow, ftxui::Color::YellowLight },
};

class RGB
{
  public:
    std::uint8_t r{};
    std::uint8_t g{};
    std::uint8_t b{};

    RGB(int r, int g, int b)
      : r(static_cast<std::uint8_t>(r))
      , g(static_cast<std::uint8_t>(g))
      , b(static_cast<std::uint8_t>(b)) {};

    RGB()
      : r(0)
      , g(0)
      , b(0) {};

    [[nodiscard]] static RGB from_hex_to_rgb(std::string_view hex);
};

class Color
{
  public:
    ColorType type{};

    NamedColor name{};
    RGB rgb{};

    std::string_view name_string{};

    Color(std::string_view string)
      : Color(from_string(string))
    {
    }

    Color() {}

    [[nodiscard]] static Color from_string(std::string_view string);
    [[nodiscard]] static Color from_rgb_array(const std::array<std::uint8_t, 3>& rgb);

    [[nodiscard]] ftxui::Color get_color_for_tui() const;

    template<typename CharT>
    static std::basic_ostream<CharT>& reset(std::basic_ostream<CharT>& stream)
    {
        stream << rang::fg::reset << rang::bg::reset << rang::style::reset;

        return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Color& color)
    {
        switch (color.type) {
            case ColorType::RGB:
                // Rang doesn't have a way to print true color colors, so we need to print the ANSI
                // escape code ourselves, we just need to hope that the user isn't using a crappy
                // terminal for an old crappy version of windows (fuck microslop).
                // We don't need to enable windows support for ANSI codes since rang already does it
                stream << "\033[38;2;" << +color.rgb.r << ';' << +color.rgb.g << ';' << +color.rgb.b
                       << 'm';
                break;
            case ColorType::NAMED:
                // LCOV_EXCL_START
                switch (color.name) {
                    case NamedColor::Black:
                        stream << rang::fg::black;
                        break;
                    case NamedColor::DarkGray:
                        stream << rang::fgB::gray;
                        break;
                    case NamedColor::LightGray:
                        stream << rang::fg::gray;
                        break;
                    case NamedColor::White:
                        // why the fuck rang doesn't have the white color support
#ifndef _WIN32
                        stream << "\033[97m";
#else
                        HANDLE handle{};

                        if (&stream == &std::cout) {
                            handle = GetStdHandle(STD_OUTPUT_HANDLE);
                        } else if (&stream == &std::cerr || &stream == &std::clog) {
                            handle = GetStdHandle(STD_ERROR_HANDLE);
                        }

                        SetConsoleTextAttribute(
                          handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#endif
                        break;
                    case NamedColor::Blue:
                        stream << rang::fg::blue;
                        break;
                    case NamedColor::LightBlue:
                        stream << rang::fgB::blue;
                        break;
                    case NamedColor::Cyan:
                        stream << rang::fg::cyan;
                        break;
                    case NamedColor::LightCyan:
                        stream << rang::fgB::cyan;
                        break;
                    case NamedColor::Green:
                        stream << rang::fg::green;
                        break;
                    case NamedColor::LightGreen:
                        stream << rang::fgB::green;
                        break;
                    case NamedColor::Magenta:
                        stream << rang::fg::magenta;
                        break;
                    case NamedColor::LightMagenta:
                        stream << rang::fgB::magenta;
                        break;
                    case NamedColor::Red:
                        stream << rang::fg::red;
                        break;
                    case NamedColor::LightRed:
                        stream << rang::fgB::red;
                        break;
                    case NamedColor::Yellow:
                        stream << rang::fg::yellow;
                        break;
                    case NamedColor::LightYellow:
                        stream << rang::fgB::yellow;
                        break;
                }
                break;
                // LCOV_EXCL_STOP
        }

        return stream;
    }
};

struct Theme
{
    Color primary{};
    Color secondary{};
    Color border{};
    Color info{};
    Color warning{};
    Color error{};

    Color directory{};
    Color executable{};
    Color symlink{};
    Color archive{};
    Color media{};
    Color normal_file{};

    Color perms_read{};
    Color perms_write{};
    Color perms_exec{};
    Color perms_null{};

    Color ls_permissions{};
    Color ls_size{};
    Color ls_user{};
    Color ls_date_modified{};
    Color ls_name{};
};

inline Theme default_theme{
    .primary   = Color("green"),
    .secondary = Color("white"),
    .border    = Color("green"),
    .info      = Color("green"),
    .warning   = Color("yellow"),
    .error     = Color("red"),

    .directory   = Color("green"),
    .executable  = Color("red"),
    .symlink     = Color("blue"),
    .archive     = Color("blue"),
    .media       = Color("yellow"),
    .normal_file = Color("white"),

    .perms_read  = Color("green"),
    .perms_write = Color("yellow"),
    .perms_exec  = Color("red"),
    .perms_null  = Color("light gray"),

    .ls_permissions   = Color("yellow"),
    .ls_size          = Color("green"),
    .ls_user          = Color("red"),
    .ls_date_modified = Color("blue"),
    .ls_name          = Color("green"),
};

inline Theme theme{ default_theme };

void
parse_theme_file();

} // namespace theme

} // namespace fima
