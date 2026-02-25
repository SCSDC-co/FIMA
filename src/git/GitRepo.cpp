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
#include <git2/branch.h>
#include <git2/commit.h>
#include <git2/global.h>
#include <git2/oid.h>
#include <git2/refs.h>
#include <git2/repository.h>
#include <git2/revwalk.h>
#include <git2/types.h>
#include <string>
#include <vector>

#include "fs/get_directories_entries.h"

namespace _fs = std::filesystem;

namespace fima {

namespace git {

GitRepo::GitRepo(const _fs::directory_entry& path)
{
    git_libgit2_init();

    this->set_repo_path(path);

    if (this->is_in_repo) {
        git_repository_open(&this->repo, this->git_repo_path.path().c_str());

        git_repository_head(&this->repo_head, this->repo);

        // for getting the HEAD of the repo
        git_reference_name_to_id(&this->oid, this->repo, git_reference_name(repo_head));

        git_commit_lookup(&this->commit, this->repo, &this->oid);

        this->set_repo_info();
        this->set_commit_info();
    }
}

void
GitRepo::set_repo_path(const std::filesystem::path& path)
{
    _fs::directory_entry _path;

    bool found{ false };

    std::vector<_fs::directory_entry> dir_entries{ fima::fs::get_directories_entries(path, true) };

    for (const _fs::directory_entry& entry : dir_entries) {
        if (entry.path().filename() == ".git") {
            _path = entry;
            found = true;
            break;
        }
    }

    if (!found) {
        set_repo_path(path.parent_path());

        return;
    }

    this->is_in_repo    = found;
    this->git_repo_path = _path;
}
void
GitRepo::change_repo_path(const std::filesystem::path& path)
{
    this->set_repo_path(path);

    if (this->is_in_repo) {
        git_repository_open(&this->repo, this->git_repo_path.path().c_str());

        git_repository_head(&this->repo_head, this->repo);

        // for getting the HEAD of the repo
        git_reference_name_to_id(&this->oid, this->repo, git_reference_name(repo_head));

        git_commit_lookup(&this->commit, this->repo, &this->oid);

        this->set_repo_info();
        this->set_commit_info();
    }
}

void
GitRepo::set_repo_info()
{
    const char* _branch;

    int _commit_number{ 0 };

    // all this shit just for getting the fucking commit number
    git_revwalk* walk;
    git_revwalk_new(&walk, this->repo);
    git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL | GIT_SORT_TIME);
    git_revwalk_push_head(walk);

    while (git_revwalk_next(&this->oid, walk) == 0) {
        _commit_number++;
    }

    git_branch_name(&_branch, repo_head);

    this->branch        = _branch;
    this->commit_number = _commit_number;
}
void
GitRepo::set_commit_info()
{
    this->commit_message   = git_commit_message(this->commit);
    this->commit_author    = git_commit_author(this->commit);
    this->commit_committer = git_commit_committer(this->commit);
}

[[nodiscard]] std::string
GitRepo::get_repo_path() const
{
    return this->git_repo_path.path().string();
}
[[nodiscard]] std::string
GitRepo::get_repo_branch() const
{
    return this->branch;
}
[[nodiscard]] std::string
GitRepo::get_commit_author() const
{
    return std::string(this->commit_author->name) + " <" + std::string(this->commit_author->email) +
           ">";
}
[[nodiscard]] std::string
GitRepo::get_commit_committer() const
{
    return std::string(this->commit_committer->name) + " <" +
           std::string(this->commit_committer->email) + ">";
}
[[nodiscard]] std::string
GitRepo::get_commit_message() const
{
    std::string message{ this->commit_message };

    while (!message.empty() &&
           std::string_view("\n\t ").find(message.back()) != std::string_view::npos) {
        message.pop_back();
    }

    return message;
}
[[nodiscard]] int
GitRepo::get_commit_number() const
{
    return this->commit_number;
}

GitRepo::~GitRepo()
{
    if (this->repo) {
        git_repository_free(this->repo);
        git_commit_free(this->commit);
    }

    git_libgit2_shutdown();
}

} // namespace git

} // namespace fima
