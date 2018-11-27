#include <fsystem_impl/error.hpp>
#include <fsystem_impl/extension.hpp>

using namespace std;

namespace fsystem {

Extension::Extension(const string& _extension): extension(_extension) {
    if (extension.empty() || (extension.size() == 1 && extension[0] == '.')) {
        throw InvalidExtensionError("Cannot create empty extension.");
    }
    if (extension[0] == '.') {
        extension = extension.substr(1);
    }
    if (extension.find('.') != string::npos) {
        throw InvalidExtensionError("Cannot create an extension that contains a '.' character.");
    }
}

Extension::Extension(const Extension& other): extension(other.extension) {}

Extension::Extension(Extension&& other) noexcept: extension(move(other.extension)) {}

Extension& Extension::operator=(const Extension& other) {
    if (this != &other) {
        extension = other.extension;
    }
    return *this;
}

Extension& Extension::operator=(Extension&& other) noexcept {
    if (this != &other) {
        extension = move(other.extension);
    }
    return *this;
}

string Extension::toString() const {
    return extension;
}

string Extension::key() const {
    return extension;
}

}
