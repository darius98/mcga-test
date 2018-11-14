#ifndef COMMON_FSYSTEM_INCLUDE_ERROR_H_
#define COMMON_FSYSTEM_INCLUDE_ERROR_H_

#include <stdexcept>


namespace fsystem {

class FSystemError: public std::runtime_error {
public:
    explicit FSystemError(const char* _what);
};

class InvalidExtensionError: public FSystemError {
public:
    explicit InvalidExtensionError(const char* _what);
};

class InvalidFileError: public FSystemError {
public:
    explicit InvalidFileError(const char* _what);
};

class InvalidFolderError: public FSystemError {
public:
    InvalidFolderError();
};

class EmptyPathError: public FSystemError {
public:
    EmptyPathError();
};

class NoExtensionError: public FSystemError {
public:
    NoExtensionError();
};

}

#endif
