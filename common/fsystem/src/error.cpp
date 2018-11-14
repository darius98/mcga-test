#include "error.hpp"

using namespace std;


namespace fsystem {

FSystemError::FSystemError(const char* _what): runtime_error(_what) {}

InvalidExtensionError::InvalidExtensionError(const char* _what):
        FSystemError(_what) {}

InvalidFileError::InvalidFileError(const char* _what): FSystemError(_what) {}

EmptyPathError::EmptyPathError():
        FSystemError("Cannot get the last component of an empty path!") {}

InvalidFolderError::InvalidFolderError(): FSystemError("Folder does not exist.") {}

NoExtensionError::NoExtensionError(): FSystemError("File has no extension.") {}

}
