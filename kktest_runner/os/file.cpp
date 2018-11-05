#include <dirent.h>
#include <sys/stat.h>

#include <fstream>

#include "error.hpp"
#include "file.hpp"

using namespace std;

namespace kktest_runner {

File::File(const string& _path): File(Path(_path)) {}

File::File(const Path& _path): path(_path.parent()) {
    string s = _path.last();
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

bool File::hasExtension() const {
    return !extensions.empty();
}

std::vector<Extension> File::getAllExtensions() const {
    return extensions;
}

Extension File::getExtension() const {
    if (extensions.empty()) {
        throw NoExtensionError();
    }
    return extensions.back();
}

std::string File::nameWithoutAllExtensions() const {
    return rawName;
}

std::string File::nameWithoutExtension() const {
    std::string ret = rawName;
    for (size_t i = 0; i + 1 < extensions.size(); ++ i) {
        ret += '.';
        ret += extensions[i].toString();
    }
    return ret;
}

string File::name() const {
    string ret = rawName;
    for (const Extension& extension: extensions) {
        ret += '.';
        ret += extension.toString();
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
        throw InvalidFileError(
            ("Unable to remove file at: " + toString()).c_str());
    }
}

std::string File::key() const {
    return toString();
}

}
