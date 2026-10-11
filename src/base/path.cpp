#include <arkanjo/base/path.hpp>

Path::Path(const fs::path& _path) {
    resource_path = _path;
}

const fs::path& Path::value() const {
    return resource_path;
}

bool Path::empty() const {
    return resource_path.empty();
}

fs::path Path::relative_path() const {
    return resource_path.parent_path();
}

bool Path::operator<(const Path& path) const {
    return resource_path < path.resource_path;
}

bool Path::operator!=(const Path& path) const {
    return resource_path != path.resource_path;
}

bool Path::operator==(const Path& path) const {
    return resource_path == path.resource_path;
}
