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
#include <git2/strarray.h>
#include <git2/types.h>
#include <string>
#include <vector>

namespace fima {

namespace git {

class GitRepo
{
  private:
    bool is_in_repo{};

    int commit_number{};

    std::string commit_message{};
    std::string branch{};
    std::string tag_name{};
    std::string tag_message{};

    std::vector<std::string> tag_list{};

    std::filesystem::path git_repo_path{};

    git_repository* repo = nullptr;
    git_commit* commit{};
    git_reference* repo_head{};
    git_tag* tag{};
    git_strarray str_array{};
    git_oid oid{ 0 };

    const git_signature* commit_author{};
    const git_signature* commit_committer{};
    const git_signature* tag_tagger{};

  public:
    GitRepo(const std::filesystem::path& path);

    void set_repo_path(const std::filesystem::path& path);
    void change_repo_path(const std::filesystem::path& path);

    void set_repo_info();
    void set_commit_info();
    void set_tag_info();

    [[nodiscard]] std::filesystem::path get_repo_path() const;

    [[nodiscard]] std::vector<std::string> get_tag_list() const;

    [[nodiscard]] std::string get_repo_branch() const;
    [[nodiscard]] std::string get_commit_message() const;
    [[nodiscard]] std::string get_commit_author() const;
    [[nodiscard]] std::string get_commit_committer() const;
    [[nodiscard]] std::string get_tag_name() const;
    [[nodiscard]] std::string get_tag_message() const;
    [[nodiscard]] std::string get_tag_tagger() const;

    [[nodiscard]] int get_commit_number() const;

    [[nodiscard]] bool is_file_ignored(const std::filesystem::path& path) const;
    [[nodiscard]] bool get_is_in_repo() const;

    ~GitRepo();
};

} // namespace git

} // namespace fima
