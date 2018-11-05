#ifndef KKTEST_RUNNER_OS_FOLDER_H_
#define KKTEST_RUNNER_OS_FOLDER_H_

#include "file.hpp"
#include "path.hpp"


namespace kktest_runner {

class Folder: public Comparable<std::string> {
public:
    static Folder currentWorkingFolder();

    static Folder parent(const Folder& folder);

    Folder(const Path& path);
    Folder(Path&& path);
    Folder(const Folder& other);
    Folder(Folder&& other);
    Folder(const std::string& str);
    Folder(std::string&& str);

    Folder& operator=(const Folder& other);
    Folder& operator=(Folder&& other);

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
