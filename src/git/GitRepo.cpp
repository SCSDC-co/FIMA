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

#include <algorithm>
#include <filesystem>
#include <git2/branch.h>
#include <git2/commit.h>
#include <git2/global.h>
#include <git2/ignore.h>
#include <git2/net.h>
#include <git2/oid.h>
#include <git2/refs.h>
#include <git2/refspec.h>
#include <git2/remote.h>
#include <git2/repository.h>
#include <git2/revwalk.h>
#include <git2/strarray.h>
#include <git2/tag.h>
#include <git2/types.h>
#include <string>
#include <vector>

#include "fs/get_directories_entries.h"
#include "fs/operations.h"

namespace _fs = std::filesystem;

namespace fima {

namespace git {

GitRepo::GitRepo(const _fs::path& path)
{
    git_libgit2_init();

    this->set_repo_path(_fs::canonical(path));

    if (this->is_in_repo) {
        git_repository_open(&this->repo, this->git_repo_path.c_str());

        git_repository_head(&this->repo_head, this->repo);

        // for getting the HEAD of the repo
        git_reference_name_to_id(&this->oid, this->repo, git_reference_name(repo_head));

        git_commit_lookup(&this->commit, this->repo, &this->oid);

        this->set_repo_info();
        this->set_commit_info();
        this->set_tag_info();
        this->set_remote_info();
    }
}

void
GitRepo::set_repo_path(const _fs::path& path)
{
    if (fima::fs::operations::is_root(path)) {
        this->is_in_repo = false;

        return;
    }

    _fs::path _path;

    bool found{ false };

    std::vector<_fs::directory_entry> dir_entries{ fima::fs::get_directories_entries_no_git(path,
                                                                                            true) };

    for (const _fs::directory_entry& entry : dir_entries) {
        if (entry.path().filename() == ".git") {
            _path = _fs::canonical(entry);
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
GitRepo::change_repo_path(const _fs::path& path)
{
    this->set_repo_path(_fs::canonical(path));

    if (this->is_in_repo) {
        git_repository_open(&this->repo, this->git_repo_path.c_str());

        git_repository_head(&this->repo_head, this->repo);

        // for getting the HEAD of the repo
        git_reference_name_to_id(&this->oid, this->repo, git_reference_name(this->repo_head));

        this->set_commit_info();
        this->set_tag_info();
        this->set_remote_info();
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
    if (this->commit) {
        git_commit_free(this->commit);

        this->commit = nullptr;
    }

    int err = git_commit_lookup(&this->commit, this->repo, &this->oid);

    if (err == 0) {
        this->commit_message   = git_commit_message(this->commit);
        this->commit_author    = git_commit_author(this->commit);
        this->commit_committer = git_commit_committer(this->commit);
    } else {
        this->commit_message   = "";
        this->commit_author    = {};
        this->commit_committer = {};
    }
}
void
GitRepo::set_tag_info()
{
    int err;

    if (this->tag) {
        git_tag_free(this->tag);

        this->tag = nullptr;
    }

    err = git_tag_lookup(&this->tag, this->repo, &this->oid);

    if (err == 0) {
        this->tag_name    = git_tag_name(this->tag);
        this->tag_message = git_tag_message(this->tag);
        this->tag_tagger  = git_tag_tagger(this->tag);
    } else {
        this->tag_name    = "-";
        this->tag_message = "-";
        this->tag_tagger  = {};
    }

    err = git_tag_list(&this->tag_array, this->repo);

    if (err == 0) {
        for (int i = 0; i < this->tag_array.count; ++i) {
            tag_list.push_back(this->tag_array.strings[i]);
        }

        std::sort(this->tag_list.begin(),
                  this->tag_list.end(),
                  [](const std::string& a, const std::string& b) { return a > b; });
    } else {
        tag_list.push_back("-");
    }
}
void
GitRepo::set_remote_info()
{
    int err;

    err = git_remote_list(&this->remote_array, this->repo);

    if (err != 0) {
        remote_list.push_back({ "-", "" });
    }

    git_remote* remote{};

    for (int i = 0; i < this->remote_array.count; ++i) {
        GitRepo::Remote _remote{ "-", "" };

        int err = git_remote_lookup(&remote, this->repo, this->remote_array.strings[i]);

        if (err == 0) {
            _remote.set_name(git_remote_name(remote));
            _remote.set_url(git_remote_url(remote));
        }

        remote_list.push_back(_remote);
    }
}

[[nodiscard]] _fs::path
GitRepo::get_repo_path() const
{
    return this->git_repo_path;
}
[[nodiscard]] std::vector<std::string>
GitRepo::get_tag_list() const
{
    return this->tag_list;
}
[[nodiscard]] std::vector<GitRepo::Remote>
GitRepo::get_remote_list() const
{
    return this->remote_list;
}
[[nodiscard]] std::string
GitRepo::get_repo_branch() const
{
    return this->branch;
}
[[nodiscard]] std::string
GitRepo::get_commit_author() const
{
    if (this->commit_author) {
        return std::string(this->commit_author->name) + " <" +
               std::string(this->commit_author->email) + ">";
    } else {
        return "-";
    }
}
[[nodiscard]] std::string
GitRepo::get_commit_committer() const
{
    if (this->commit_committer) {
        return std::string(this->commit_committer->name) + " <" +
               std::string(this->commit_committer->email) + ">";
    } else {
        return "-";
    }
}
[[nodiscard]] std::string
GitRepo::get_commit_message() const
{
    std::string message{ this->commit_message };

    message.erase(std::remove_if(message.begin(),
                                 message.end(),
                                 [](const char& c) { return c == '\n' || c == '\r'; }),
                  message.end());

    return message;
}
[[nodiscard]] std::string
GitRepo::get_tag_name() const
{
    return this->tag_name;
}
[[nodiscard]] std::string
GitRepo::get_tag_message() const
{
    return this->tag_message;
}
[[nodiscard]] std::string
GitRepo::get_tag_tagger() const
{
    if (this->tag_tagger) {
        return std::string(this->tag_tagger->name) + " <" + std::string(this->tag_tagger->email) +
               ">";
    } else {
        return "-";
    }
}
[[nodiscard]] int
GitRepo::get_commit_number() const
{
    return this->commit_number;
}

[[nodiscard]] bool
GitRepo::is_file_ignored(const _fs::path& path) const
{
    int is_ignored;

    _fs::path relative_path =
      _fs::relative(_fs::canonical(path), this->git_repo_path.parent_path());

    git_ignore_path_is_ignored(&is_ignored, this->repo, relative_path.c_str());

    return is_ignored;
}
[[nodiscard]] bool
GitRepo::get_is_in_repo() const
{
    return this->is_in_repo;
}

GitRepo::~GitRepo()
{
    if (this->repo) {
        git_repository_free(this->repo);
        git_commit_free(this->commit);
        git_tag_free(this->tag);
        git_strarray_dispose(&this->tag_array);
        git_strarray_dispose(&this->remote_array);
    }

    git_libgit2_shutdown();
}

} // namespace git

} // namespace fima
