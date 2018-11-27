#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <cstdio>
#include <cstring>

#include <fsystem_impl/error.hpp>
#include <fsystem_impl/folder.hpp>

using namespace std;

namespace fsystem {

Folder Folder::currentWorkingFolder() {
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    return Folder(buff);
}

Folder Folder::parent(const Folder& folder) {
    return Folder(folder.path.parent());
}

Folder::Folder(const Path& path): path(path.absolute()) {}

Folder::Folder(Path&& path): path(path.absolute()) {}

Folder::Folder(const Folder& other): path(other.path) {}

Folder::Folder(Folder&& other) noexcept: path(move(other.path)) {}

Folder::Folder(const string& str): path(Path(str).absolute()) {}

Folder::Folder(string&& str): path(Path(str).absolute()) {}

Folder& Folder::operator=(const Folder& other) {
    path = other.path;
    return *this;
}

Folder& Folder::operator=(Folder&& other) noexcept {
    path = move(other.path);
    return *this;
}

string Folder::last() const {
    return path.last();
}

string Folder::toString() const {
    return path.toString();
}

Folder Folder::parent() const {
    return Folder(path.parent());
}

Path Folder::getPath() const {
    return path;
}

bool Folder::exists() const {
    struct stat info;
    int errCode = stat(toString().c_str(), &info);
    if(errCode != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

pair<vector<File>, vector<Folder>> Folder::children() const {
    vector<File> files;
    vector<Folder> folders;

    DIR* cFolder = opendir(path.toString().c_str());
    if (cFolder == nullptr) {
        throw InvalidFolderError();
    }

    for (dirent* cChildPath = readdir(cFolder); cChildPath != nullptr;
            cChildPath = readdir(cFolder)) {
        if (strcmp(cChildPath->d_name, ".") == 0 ||
            strcmp(cChildPath->d_name, "..") == 0) {
            continue;
        }
        Path childPath = Path::join(path, cChildPath->d_name);

        struct stat info;
        int errCode = stat(childPath.toString().c_str(), &info);
        if (errCode != 0) {
            files.emplace_back(childPath);
            continue;
        }
        if (info.st_mode & S_IFDIR) {
            folders.emplace_back(childPath);
        }
        if (info.st_mode & S_IFREG) {
            files.emplace_back(childPath);
        }
    }
    (void)closedir(cFolder);

    return {files, folders};
}

string Folder::key() const {
    return toString();
}

}
