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

#include <filesystem>
#include <vector>

#include "fs/operations.h"
#include "libzippp/libzippp.h"

namespace fima {

namespace fs {

namespace archive {

using namespace libzippp;

void
zip(const std::vector<std::filesystem::path>& items_to_zip, const std::filesystem::path& output)
{
    ZipArchive archive(output.string());

    archive.open(ZipArchive::New);

    for (const auto& item : items_to_zip) {
        if (std::filesystem::is_directory(item)) {
            for (auto& entry : std::filesystem::recursive_directory_iterator(item)) {
                if (entry.is_regular_file()) {
                    std::filesystem::path relative =
                      std::filesystem::relative(entry.path(), item.parent_path());

                    archive.addFile(relative.string(), entry.path().string());
                }
            }
        } else if (std::filesystem::is_regular_file(item)) {
            archive.addFile(item.filename().string(), item.string());
        }
    }

    archive.close();
}

void
unzip(const std::filesystem::path& archive_to_unzip, const std::filesystem::path& output)
{
    ZipArchive archive(archive_to_unzip.string());
    archive.open(ZipArchive::ReadOnly);

    auto entries = archive.getEntries();

    for (auto& entry : entries) {
        std::filesystem::path out_path = output / entry.getName();

        if (entry.isDirectory()) {
            std::filesystem::create_directories(out_path);
        } else {
            if (out_path.has_parent_path()) {
                std::filesystem::create_directories(out_path.parent_path());
            }

            fima::fs::operations::create(out_path, entry.readAsText());
        }
    }

    archive.close();
}

} // namespace archive

} // namespace fs

} // namespace fima
