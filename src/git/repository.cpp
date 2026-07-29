#include <arkanjo/git/repository.hpp>

GitRepository::GitRepository(const fs::path& path) {
    git_libgit2_init();

    git_repository* repo = nullptr;

    int error = git_repository_open_ext(
        &repo,
        path.string().c_str(),
        0,
        nullptr
    );

    if (error != 0)
        return;

    repository = repo;
}

GitRepository::~GitRepository() {
    if (repository)
        git_repository_free(repository);

    git_libgit2_shutdown();
}

bool GitRepository::valid() const {
    return repository != nullptr;
}

std::filesystem::path GitRepository::root() const {
    const char* path = git_repository_workdir(repository);

    if (!path)
        return {};

    return path;
}

std::string GitRepository::head_commit() const {
    git_oid oid;

    git_reference* head = nullptr;

    if (git_repository_head(&head, repository) != 0)
        return {};

    const git_oid* target = git_reference_target(head);

    std::string result = git_oid_tostr_s(target);

    git_reference_free(head);

    return result;
}
