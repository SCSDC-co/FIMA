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

#include "fs/FileDetector.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <magic.h>

#include "theme.h"

namespace fima {

namespace fs {

[[nodiscard]] std::string
FileDetector::get_encoding(const std::filesystem::path& path)
{
    std::string encoding{};

#ifndef _WIN32
    std::string result{ this->run_file_with_flags(MAGIC_MIME_ENCODING | MAGIC_SYMLINK, path) };

    if (result == "-1") {
        std::cerr << fima::theme::theme.error
                  << "Error while getting the encoding of: " << fima::theme::Color::reset
                  << path.string() << '\n';

        std::cerr << magic_error(this->magic) << '\n';

        std::exit(magic_errno(this->magic));
    }

    encoding = result;
#endif

    return encoding;
}
[[nodiscard]] std::string
FileDetector::get_mime(const std::filesystem::path& path)
{
    std::string mime{};

#ifndef _WIN32
    std::string result{ this->run_file_with_flags(MAGIC_MIME_TYPE | MAGIC_SYMLINK, path) };

    if (result == "-1") {
        std::cerr << fima::theme::theme.error
                  << "Error while getting the MIME type of: " << fima::theme::Color::reset
                  << path.string() << '\n';

        std::cerr << magic_error(this->magic) << '\n';

        std::exit(magic_errno(this->magic));
    }

    mime = result;
#endif

    return mime;
}

[[nodiscard]] bool
FileDetector::is_file_binary(const std::filesystem::path& path)
{
    bool is_binary{ true };

#ifndef _WIN32
    is_binary = this->get_encoding(path) == "binary";
#endif

    return is_binary;
}
[[nodiscard]] bool
FileDetector::is_file_clocable(const std::filesystem::path& path)
{
    bool is_clocable{ true };

#ifndef _WIN32
    is_clocable = !this->is_file_binary(path);
#endif

    return is_clocable && !this->is_file_lockfile(path);
}

[[nodiscard]] bool
FileDetector::is_file_executable(const std::filesystem::path& path)
{
    if (std::filesystem::is_directory(path)) {
        return false;
    }

    if (this->is_file_binary(path)) {
        return true;
    }

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    // scripts are text even when executable
    if (scripts.contains(ext)) {
        return false;
    }

    if (executables.contains(ext)) {
        return true;
    }

    auto st = std::filesystem::status(path);

    std::filesystem::perms permissions{ st.permissions() };

    return (permissions & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ||
           (permissions & std::filesystem::perms::group_exec) != std::filesystem::perms::none ||
           (permissions & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
}
[[nodiscard]] bool
FileDetector::is_file_media(const std::filesystem::path& path)
{
    bool is_media{ true };

#ifndef _WIN32
    std::string mime{ this->get_mime(path) };

    is_media =
      mime.starts_with("audio/") || mime.starts_with("image/") || mime.starts_with("video/");
#endif

    return is_media;
}
[[nodiscard]] bool
FileDetector::is_file_lockfile(const std::filesystem::path& path)
{
    std::string name{ path.filename().string() };
    std::string ext{ path.extension().string() };

    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    return ext == ".lock" || lockfiles.contains(name);
}
[[nodiscard]] bool
FileDetector::is_compressed_archive(const std::filesystem::path& path)
{
    bool is_archive{ true };

#ifndef _WIN32
    std::string mime{ this->get_mime(path) };

    is_archive = archive_mimes.contains(mime);
#endif

    return is_archive;
}

FileDetector file_detector{};

} // namespace fs

} // namespace fima
