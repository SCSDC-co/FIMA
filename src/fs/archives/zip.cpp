/*
 * scr/fs/archive.cpp
 * include/fs/archive.h
 *
 * A module that contains some useful functions for working with archives
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/archives/zip.h"

#include <filesystem>
#include <libzippp/libzippp.h>
#include <vector>

namespace fima {

namespace fs {

namespace archives {

namespace zip {

using namespace libzippp;

void
create_archive(const std::vector<std::filesystem::path>& items_to_zip,
               const std::filesystem::path& output)
{
    ZipArchive archive(output.string());

    archive.open(ZipArchive::New);

    for (const auto& item : items_to_zip) {
        if (std::filesystem::is_directory(item)) {
            archive.addEntry(item);

            for (auto& entry : std::filesystem::recursive_directory_iterator(
                   item, std::filesystem::directory_options::skip_permission_denied)) {
                if (entry.is_regular_file()) {
                    archive.addFile(entry.path().string(), entry.path().string());
                } else if (entry.is_directory()) {
                    archive.addEntry(entry.path());
                }
            }
        } else if (std::filesystem::is_regular_file(item)) {
            archive.addFile(item.string(), item.string());
        }
    }

    archive.close();
}

} // namespace zip

} // namespace archives

} // namespace fs

} // namespace fima
