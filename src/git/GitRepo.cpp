/*
 * src/git/GitRepo.cpp
 * include/git/GitRepo.h
 *
 * The class that contains the RAII wrapper for libgit2
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "git/GitRepo.h"

#include <filesystem>
#include <git2/global.h>
#include <git2/repository.h>
#include <string>

namespace fs = std::filesystem;

namespace fima {

namespace git {

GitRepo::GitRepo(const fs::directory_entry& path)
{
    git_libgit2_init();

    int err = git_repository_open_ext(&this->repo, path.path().c_str(), 0, NULL);

    if (err < 0) {
        this->is_in_repo = false;
    }

    if (this->is_in_repo) {
        this->set_git_dir_path();

        git_repository_init(&this->repo, this->git_repo_path.c_str(), false);
    }
}

void
GitRepo::set_git_dir_path()
{
    this->git_repo_path = git_repository_path(this->repo);
}

[[nodiscard]] fs::path
GitRepo::get_repo_path() const
{
    return this->git_repo_path;
}

[[nodiscard]] std::string
GitRepo::get_repo_branch() const
{
    return this->branch;
}

[[nodiscard]] std::string
GitRepo::get_commit_author() const
{
    return this->commit_author;
}

[[nodiscard]] std::string
GitRepo::get_commit_message() const
{
    return this->commit_message;
}

[[nodiscard]] int
GitRepo::get_commit_number() const
{
    return this->commit_number;
}

GitRepo::~GitRepo()
{
    if (is_in_repo) {
        if (this->repo) {
            git_repository_free(this->repo);
        }
    }

    git_libgit2_shutdown();
}

} // namespace git

} // namespace fima
