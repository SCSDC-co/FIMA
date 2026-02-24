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

#pragma once

#include <filesystem>
#include <git2/oid.h>
#include <git2/types.h>
#include <string>

namespace fima {

namespace git {

class GitRepo
{
  private:
    bool is_in_repo{};

    int commit_number{ 0 };

    std::string branch{};
    std::string commit_message{};

    std::filesystem::path git_repo_path{};

    git_repository* repo = NULL;
    git_commit* commit{};
    git_oid oid{ 0 };

    const git_signature* commit_author{};
    const git_signature* commit_committer{};

  public:
    GitRepo(const std::filesystem::directory_entry& path);

    void set_git_dir_path();
    void set_commit_info();
    void set_commit_number();

    [[nodiscard]] std::filesystem::path get_repo_path() const;

    [[nodiscard]] std::string get_repo_branch() const;
    [[nodiscard]] std::string get_commit_message() const;
    [[nodiscard]] std::string get_commit_author() const;
    [[nodiscard]] std::string get_commit_committer() const;

    [[nodiscard]] int get_commit_number() const;

    ~GitRepo();
};

} // namespace git

} // namespace fima
