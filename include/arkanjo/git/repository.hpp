#pragma once

#include <git2.h>
#include <filesystem>

namespace fs = std::filesystem;

class GitRepository {
  public:
    explicit GitRepository(const fs::path& path);

    ~GitRepository();

    GitRepository(const GitRepository&) = delete;
    GitRepository& operator=(const GitRepository&) = delete;

    bool valid() const;

    fs::path root() const;

    std::string head_commit() const;

  private:
    git_repository* repository = nullptr;
};
