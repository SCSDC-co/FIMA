/*
 * src/commands/info.cpp
 * include/commands/info.h
 *
 * A command that gives information about a file/directory
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/info/info.h"

#include <filesystem>

#include "commands/info/directory/info.h"
#include "commands/info/file/info.h"
#include "commands/info/git/info.h"
#include "logger.h"
#include "utility/colors.h"

namespace fima {

namespace info {

void
info(const fima::options::info_options& options, fima::git::GitRepo& repo)
{
    if (!options.path.exists()) {
        fima::logger::error(true,
                            "info",
                            fima::colors::RED + "The path doesn't exist: " + fima::colors::RESET +
                              "{}",
                            options.path.path().string());

        return;
    }

    if (options.git) {
        repo.change_repo_path(options.path);

        git::info(options.path, repo);

        return;
    }

    if (options.path.is_directory()) {
        dir::info(options.path);
    } else {
        file::info(options.path);
    }
}

} // namespace info

} // namespace fima
