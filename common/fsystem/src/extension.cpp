#include "error.hpp"
#include "extension.hpp"

using namespace std;

namespace fsystem {

Extension::Extension(const string& _extension) {
    if (_extension.empty() ||
            (_extension.size() == 1 && _extension[0] == '.')) {
        throw InvalidExtensionError("Cannot create empty extension.");
    }
    extension = _extension;
    if (extension[0] == '.') {
        extension = extension.substr(1);
    }
    if (extension.find('.') != string::npos) {
        throw InvalidExtensionError(
            "Cannot create an extension that contains a '.' character."
        );
    }
}

Extension::Extension(const Extension& other):
        extension(other.extension) {}

Extension::Extension(Extension&& other) noexcept:
        extension(move(other.extension)) {}

Extension& Extension::operator=(const Extension& other) {
    extension = other.extension;
    return *this;
}

Extension& Extension::operator=(Extension&& other) noexcept {
    extension = move(other.extension);
    return *this;
}

string Extension::toString() const {
    return extension;
}

string Extension::key() const {
    return extension;
}

}
