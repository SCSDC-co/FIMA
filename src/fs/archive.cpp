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

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>
#include <iostream>
#include <sstream>
#include <vector>

namespace fima {

namespace fs {

namespace archive {

void
zip(const std::vector<std::filesystem::path>& files_to_zip, const std::filesystem::path& output)
{
    std::string data{};

    std::ostringstream ss;

    for (const std::filesystem::path& item : files_to_zip) {
        std::ifstream file(item);

        ss << file.rdbuf();
    }

    data = ss.str();

    const char* data_pointer{ data.data() };
    std::size_t data_size{ data.size() };

    if (gzip::is_compressed(data_pointer, data_size)) {
        std::cout << "the data is already compressed" << '\n';
        return;
    }

    std::string compressed_data{ gzip::compress(data_pointer, data_size) };

    std::ofstream output_file(output);

    output_file << compressed_data;

    output_file.close();

    std::cout << "compressed data to: " << output.string() << '\n';
}

void
unzip(const std::filesystem::path& file_to_unzip, const std::filesystem::path& output)
{
    std::string data{};

    std::ostringstream ss;

    std::ifstream file(file_to_unzip);

    ss << file.rdbuf();

    data = ss.str();

    const char* compressed_file{ data.data() };
    std::size_t compressed_file_size{ data.size() };

    if (!gzip::is_compressed(compressed_file, compressed_file_size)) {
        std::cout << "the file isn't compressed: " << file_to_unzip.string() << '\n';

        return;
    }

    std::string decompressed_data{ gzip::decompress(compressed_file, compressed_file_size) };

    std::ofstream output_file(output);

    output_file << decompressed_data;

    output_file.close();

    std::cout << "decompressed data to: " << output.string() << '\n';
}

} // namespace archive

} // namespace fs

} // namespace fima
