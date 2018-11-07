#include "error.hpp"

using namespace std;


namespace kktest_runner {

OSError::OSError(const char* _what): runtime_error(_what) {}

InvalidExtensionError::InvalidExtensionError(const char* _what):
        OSError(_what) {}

InvalidFileError::InvalidFileError(const char* _what): OSError(_what) {}

EmptyPathError::EmptyPathError():
        OSError("Cannot get the last component of an empty path!") {}

InvalidFolderError::InvalidFolderError(): OSError("Folder does not exist.") {}

NoExtensionError::NoExtensionError(): OSError("File has no extension.") {}

}
