#include <dirent.h>
#include <sys/stat.h>

#include <fstream>

#include <fsystem_impl/error.hpp>
#include <fsystem_impl/file.hpp>

using std::fstream;
using std::ios;
using std::move;
using std::string;
using std::vector;

namespace fsystem {

File::File(const string& _path): File(Path(_path)) {}

File::File(const Path& _path): path(_path.absolute().parent()) {
    string s = _path.absolute().last();
    vector<string> v;
    size_t prev = 0, pos = 0;
    while ((pos = s.find('.', pos)) != string::npos) {
        string part = s.substr(prev, pos - prev);
        if (!part.empty()) {
            v.emplace_back(move(part));
        } else if (prev != 0) {
            throw InvalidExtensionError(
                "Cannot have a file with empty extension!");
        }
        prev = ++pos;
    }
    if (pos != prev) {
        v.push_back(s.substr(prev, pos - prev));
    } else if (prev != 0) {
        throw InvalidExtensionError("Cannot have a file with empty extension!");
    }
    if (v.empty()) {
        throw EmptyPathError();
    }
    rawName = (s[0] == '.' ? "." : "") + v[0];
    for (size_t i = 1; i < v.size(); ++ i) {
        extensions.emplace_back(v[i]);
    }
}

File::File(const File& other):
        path(other.path),
        rawName(other.rawName),
        extensions(other.extensions) {}

File::File(File&& other) noexcept:
        path(move(other.path)),
        rawName(move(other.rawName)),
        extensions(move(other.extensions)) {}

File& File::operator=(const File& other) {
    if (this != &other) {
        path = other.path;
        rawName = other.rawName;
        extensions = other.extensions;
    }
    return *this;
}

File& File::operator=(File&& other) noexcept {
    if (this != &other) {
        path = move(other.path);
        rawName = move(other.rawName);
        extensions = move(other.extensions);
    }
    return *this;
}

bool File::hasExtension() const {
    return !extensions.empty();
}

vector<Extension> File::getAllExtensions() const {
    return extensions;
}

Extension File::getExtension() const {
    if (extensions.empty()) {
        throw NoExtensionError();
    }
    return extensions.back();
}

string File::nameWithoutAllExtensions() const {
    return rawName;
}

string File::nameWithoutExtension() const {
    string ret = rawName;
    for (size_t i = 0; i + 1 < extensions.size(); ++ i) {
        ret += '.' + extensions[i].toString();
    }
    return ret;
}

string File::name() const {
    string ret = rawName;
    for (const Extension& extension: extensions) {
        ret += '.' + extension.toString();
    }
    return ret;

}

Path File::getPath() const {
    return path + name();
}

Path File::getFolderPath() const {
    return path;
}

bool File::exists() const {
    struct stat info;
    int errCode = stat(toString().c_str(), &info);
    if(errCode != 0) {
        return false;
    }
    return (info.st_mode & S_IFREG) != 0;
}

bool File::isExecutable() const {
    struct stat info;
    int errCode = stat(toString().c_str(), &info);
    if(errCode != 0) {
        return false;
    }
    return (info.st_mode & S_IXUSR) != 0;
}

bool File::isReadable() const {
    struct stat info;
    int errCode = stat(toString().c_str(), &info);
    if(errCode != 0) {
        return false;
    }
    return (info.st_mode & S_IRUSR) != 0;
}

string File::toString() const {
    return path.toString() + name();
}

void File::touch() const {
    try {
        fstream fs;
        fs.open(toString(), ios::app);
        fs.close();
    } catch(...) {
        throw InvalidFileError(
            ("Unable to touch file at: " + toString()).c_str()
        );
    }
}

void File::remove() const {
    if (::remove(toString().c_str())) {
        throw InvalidFileError(("Unable to remove file at: " + toString()).c_str());
    }
}

string File::key() const {
    return toString();
}

}
