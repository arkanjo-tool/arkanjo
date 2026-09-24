#pragma once

#include <git2.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/**
 * @brief RAII wrapper around a libgit2 git_repository handle.
 */
class GitRepository {
  public:
    /**
     * @brief Opens a Git repository at the specified filesystem path.
     * @param path Directory path within or at the root of the repository.
     */
    explicit GitRepository(const fs::path& path);

    ~GitRepository();

    GitRepository(const GitRepository&) = delete;
    GitRepository& operator=(const GitRepository&) = delete;

    /**
     * @brief Checks whether the repository was successfully opened and is valid.
     * @return True if valid, false otherwise.
     */
    bool valid() const;

    /**
     * @brief Retrieves the working directory root path of the repository.
     * @return Filesystem path to the repository root.
     */
    fs::path root() const;

    /**
     * @brief Retrieves the commit SHA string of the current HEAD.
     * @return Hexadecimal SHA string of HEAD commit.
     */
    std::string head_commit() const;

  private:
    git_repository* repository = nullptr;
};
