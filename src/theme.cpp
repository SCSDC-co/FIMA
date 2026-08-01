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

#include "theme.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <ftxui/screen/color.hpp>
#include <iostream>
#include <string_view>
#include <toml++/impl/node.hpp>
#include <toml++/impl/node_view.hpp>
#include <toml++/toml.hpp>
#include <unordered_map>

#include "config.h"

namespace fima {

namespace theme {

[[nodiscard]] RGB
RGB::from_hex_to_rgb(std::string_view hex)
{
    RGB rgb{};

    rgb.r = char_to_number_hex.at(tolower(hex[1])) * 16 + char_to_number_hex.at(tolower(hex[2]));
    rgb.g = char_to_number_hex.at(tolower(hex[3])) * 16 + char_to_number_hex.at(tolower(hex[4]));
    rgb.b = char_to_number_hex.at(tolower(hex[5])) * 16 + char_to_number_hex.at(tolower(hex[6]));

    return rgb;
}

[[nodiscard]] ftxui::Color
Color::get_color_for_tui() const
{
    ftxui::Color color{};

    switch (this->type) {
        case ColorType::RGB:
            color = ftxui::Color(this->rgb.r, this->rgb.g, this->rgb.b);
            break;
        case ColorType::NAMED:
            color = named_to_ftxui_color.at(this->name);
            break;
    }

    return color;
}

[[nodiscard]] Color
Color::from_string(std::string_view string)
{
    Color color{};

    if (string[0] == '#') {
        color.type = ColorType::RGB;

        color.rgb = RGB::from_hex_to_rgb(string);
    } else {
        color.type = ColorType::NAMED;

        color.name        = name_to_named_color.at(string);
        color.name_string = string;
    }

    return color;
}
[[nodiscard]] Color
Color::from_rgb_array(const std::array<std::uint8_t, 3>& rgb)
{
    Color color{};

    color.type = ColorType::RGB;
    color.rgb  = RGB(rgb[0], rgb[1], rgb[2]);

    return color;
}

Color
parse_color(const toml::node_view<toml::node>& node)
{
    Color color{};

    if (!node.is_array() && !node.is_string()) {
        std::cerr << theme.error
                  << "Color format not supported. Supported formats: " << Color::reset
                  << "HEX string, color name, RGB array." << '\n';

        std::exit(1);
    }

    if (node.is_array()) {
        std::array<int, 3> rgb_arr{};

        rgb_arr[0] = node.as_array()->at(0).value_or(0);
        rgb_arr[1] = node.as_array()->at(1).value_or(0);
        rgb_arr[2] = node.as_array()->at(2).value_or(0);

        for (auto item : rgb_arr) {
            if (item < 0 || item > 255) {
                std::cerr << theme.error
                          << "Value is either too high or too low, it must be between 0 and 255"
                          << Color::reset << '\n';

                std::exit(1);
            }
        }

        color = Color::from_rgb_array({ static_cast<std::uint8_t>(rgb_arr[0]),
                                        static_cast<std::uint8_t>(rgb_arr[1]),
                                        static_cast<std::uint8_t>(rgb_arr[2]) });
    } else if (auto string{ node.value<std::string_view>() };
               string && !string->empty() && string->at(0) == '#') {
        if (string->length() != 7) {
            std::cerr << theme.error << "HEX string is not valid." << Color::reset << '\n';
            std::cerr << theme.error << "Reason: " << Color::reset
                      << "string is not of the correct length." << '\n';

            std::exit(1);
        }

        for (int i{ 1 }; i < string->length(); ++i) {
            char c{ static_cast<char>(std::tolower(string->at(i))) };

            if (!char_to_number_hex.contains(c)) {
                std::cerr << theme.error << "HEX string is not valid." << '\n';
                std::cerr << "Reason: " << Color::reset << '\'' << c << '\''
                          << " is not a valid hexadecimal character." << '\n';

                std::exit(1);
            }
        }

        color = Color::from_string(*string);
    } else {
        std::string lowercase{ *string };

        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);

        if (!name_to_named_color.contains(lowercase)) {
            std::cerr << '\"' << *string << '\"' << theme.error << " is not a valid color name."
                      << Color::reset << '\n';

            std::exit(1);
        }

        color = Color::from_string(lowercase);
    }

    return color;
}

void
apply_color_if_node_exists(Color& color, const toml::node_view<toml::node>& node)
{
    if (node) {
        color = parse_color(node);
    }
}

void
parse_theme_file()
{
    if (!std::filesystem::exists(fima::config::THEME_FILE_PATH)) {
        return;
    }

    auto theme_file{ toml::parse_file(fima::config::THEME_FILE_PATH.string()) };

    auto general{ theme_file["general"] };
    auto specific{ theme_file["specific"] };
    auto perms{ theme_file["permissions"] };
    auto ls{ theme_file["ls"] };

    apply_color_if_node_exists(theme.primary, general["primary"]);
    apply_color_if_node_exists(theme.secondary, general["secondary"]);
    apply_color_if_node_exists(theme.border, general["border"]);
    apply_color_if_node_exists(theme.info, general["info"]);
    apply_color_if_node_exists(theme.warning, general["warning"]);
    apply_color_if_node_exists(theme.error, general["error"]);

    apply_color_if_node_exists(theme.directory, specific["directory"]);
    apply_color_if_node_exists(theme.executable, specific["executable"]);
    apply_color_if_node_exists(theme.symlink, specific["symlink"]);
    apply_color_if_node_exists(theme.archive, specific["archive"]);
    apply_color_if_node_exists(theme.media, specific["media"]);
    apply_color_if_node_exists(theme.normal_file, specific["normal_file"]);

    apply_color_if_node_exists(theme.perms_read, perms["read"]);
    apply_color_if_node_exists(theme.perms_write, perms["write"]);
    apply_color_if_node_exists(theme.perms_exec, perms["execute"]);
    apply_color_if_node_exists(theme.perms_null, perms["null"]);

    apply_color_if_node_exists(theme.ls_permissions, ls["permissions"]);
    apply_color_if_node_exists(theme.ls_size, ls["size"]);
    apply_color_if_node_exists(theme.ls_user, ls["user"]);
    apply_color_if_node_exists(theme.ls_date_modified, ls["date_modified"]);
    apply_color_if_node_exists(theme.ls_name, ls["name"]);
}

} // namespace theme

} // namespace fima
