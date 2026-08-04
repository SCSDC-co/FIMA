/*
 * scr/fs/archives/unzip.cpp
 * include/fs/archives/unzip.h
 *
 * A utility to extract archives. It is a separate module because it needs to support many archive
 * formats.
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/archives/unzip.h"

#include <filesystem>
#include <libzippp/libzippp.h>

#include "fs/operations.h"

namespace fima {

namespace fs {

namespace archives {

using namespace libzippp;

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

} // namespace archives

} // namespace fs

} // namespace fima
