#ifndef KKTEST_RUNNER_OS_FILE_H_
#define KKTEST_RUNNER_OS_FILE_H_

#include <string>

#include <utils/comparable.hpp>
#include "extension.hpp"
#include "path.hpp"


namespace kktest_runner {

class File: public Comparable<std::string> {
public:
    explicit File(const std::string& path);
    explicit File(const Path& path);
    File(const File& other);
    File(File&& other) noexcept;

    File& operator=(const File& other);
    File& operator=(File&& other) noexcept;

    bool hasExtension() const;
    std::vector<Extension> getAllExtensions() const;
    Extension getExtension() const;

    std::string nameWithoutAllExtensions() const;
    std::string nameWithoutExtension() const;
    std::string name() const;

    Path getPath() const;
    Path getFolderPath() const;

    bool exists() const;
    bool isExecutable() const;

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
