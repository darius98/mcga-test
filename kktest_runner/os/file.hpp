#ifndef KKTEST_RUNNER_OS_FILE_H_
#define KKTEST_RUNNER_OS_FILE_H_

#include <string>

#include <utils/comparable.hpp>
#include "extension.hpp"
#include "path.hpp"


namespace kktest_runner {

class File: public Comparable<std::string> {
public:
    File(const std::string& path);
    File(const Path& path);

    bool hasExtension() const;
    std::vector<Extension> getAllExtensions() const;
    Extension getExtension() const;

    std::string nameWithoutAllExtensions() const;
    std::string nameWithoutExtension() const;
    std::string name() const;

    Path getPath() const;
    Path getFolderPath() const;

    bool exists() const;

    std::string toString() const;

    void touch() const;
    void remove() const;

private:
    Path path;
    std::string rawName;
    std::vector<Extension> extensions;

    std::string key() const override;
};

}

#endif
