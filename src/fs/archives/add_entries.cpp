/*
 * scr/fs/archives/add_entries.cpp
 * include/fs/archives/add_entries.h
 *
 * Adds entries to a .zip archive
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "fs/archives/add_entries.h"

#include <filesystem>
#include <fstream>
#include <libzippp/libzippp.h>
#include <vector>

namespace fima {

namespace fs {

namespace archives {

namespace zip {

using namespace libzippp;

void
add_entries(const std::vector<std::filesystem::path>& entries_to_add,
            const std::filesystem::path& archive_path)
{
    ZipArchive archive(archive_path.string());
    archive.open(ZipArchive::Write);

    for (const auto& item : entries_to_add) {
        if (std::filesystem::is_directory(item)) {
            archive.addEntry(item.string());
        } else if (std::filesystem::is_regular_file(item)) {
            std::ofstream file(item);

            std::stringstream buffer{};

            buffer << file.rdbuf();

            archive.addData(item.string(), buffer.str().c_str(), buffer.str().size());
        }
    }

    archive.close();
}

} // namespace zip

} // namespace archives

} // namespace fs

} // namespace fima
