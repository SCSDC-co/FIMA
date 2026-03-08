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
  public:
    class Remote
    {
      public:
        Remote(const std::string& _name, const std::string& _url)
          : name(_name)
          , url(_url)
        {
        }

        void set_name(const std::string& name) { this->name = name; }
        void set_url(const std::string& url) { this->url = url; }

        [[nodiscard]] std::string get_name() const { return this->name; }
        [[nodiscard]] std::string get_url_bare() const { return this->url; }
        [[nodiscard]] std::string get_url() const { return "<" + this->url + ">"; }

        bool operator==(const Remote& other) const
        {
            return this->get_name() == other.get_name() && this->url == other.url;
        }

      private:
        std::string name{};
        std::string url{};
    };

    GitRepo(const std::filesystem::path& path);

    void set_repo_path(const std::filesystem::path& path);
    void change_repo_path(const std::filesystem::path& path);

    void set_repo_info();
    void set_commit_info();
    void set_tag_info();
    void set_remote_info();

    [[nodiscard]] std::filesystem::path get_repo_path() const;

    [[nodiscard]] std::vector<std::string> get_tag_list() const;
    [[nodiscard]] std::vector<Remote> get_remote_list() const;

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

  private:
    bool is_in_repo{};

    int commit_number{};

    std::string commit_message{};
    std::string branch{};
    std::string tag_name{};
    std::string tag_message{};

    std::vector<std::string> tag_list{};
    std::vector<Remote> remote_list{};

    std::filesystem::path git_repo_path{};

    git_repository* repo = nullptr;
    git_commit* commit{};
    git_reference* repo_head{};
    git_tag* tag{};
    git_strarray tag_array{};
    git_strarray remote_array{};
    git_oid oid{ 0 };

    const git_signature* commit_author{};
    const git_signature* commit_committer{};
    const git_signature* tag_tagger{};
};

} // namespace git

} // namespace fima
