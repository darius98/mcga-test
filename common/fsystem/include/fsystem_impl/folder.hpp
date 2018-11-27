#ifndef COMMON_FSYSTEM_INCLUDE_FSYSTEM_IMPL_FOLDER_H_
#define COMMON_FSYSTEM_INCLUDE_FSYSTEM_IMPL_FOLDER_H_

#include <fsystem_impl/file.hpp>
#include <fsystem_impl/path.hpp>

namespace fsystem {

class Folder: public strutil::StringKeyComparable {
public:
    static Folder currentWorkingFolder();

    static Folder parent(const Folder& folder);

    explicit Folder(const Path& path);
    explicit Folder(Path&& path);
    Folder(const Folder& other);
    Folder(Folder&& other) noexcept;
    explicit Folder(const std::string& str);
    explicit Folder(std::string&& str);

    Folder& operator=(const Folder& other);
    Folder& operator=(Folder&& other) noexcept;

    std::string last() const;

    bool exists() const;

    std::string toString() const;

    Folder parent() const;

    Path getPath() const;

    std::pair<std::vector<File>, std::vector<Folder>> children() const;

private:
    Path path;

    std::string key() const override;
};

}

#endif
