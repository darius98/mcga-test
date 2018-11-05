#ifndef KKTEST_RUNNER_OS_ERROR_H_
#define KKTEST_RUNNER_OS_ERROR_H_

#include <stdexcept>


namespace kktest_runner {

class OSError: public std::runtime_error {
public:
    OSError(const char* _what);
};

class InvalidExtensionError: public OSError {
public:
    InvalidExtensionError(const char* _what);
};

class InvalidFileError: public OSError {
public:
    InvalidFileError(const char* _what);
};

class InvalidFolderError: public OSError {
public:
    InvalidFolderError();
};

class EmptyPathError: public OSError {
public:
    EmptyPathError();
};

class NoExtensionError: public OSError {
public:
    NoExtensionError();
};

}

#endif
