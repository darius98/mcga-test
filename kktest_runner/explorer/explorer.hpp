#ifndef KKTEST_RUNNER_EXPLORER_EXPLORER_H_
#define KKTEST_RUNNER_EXPLORER_EXPLORER_H_

#include <os/file.hpp>
#include <os/folder.hpp>


namespace kktest_runner {

class Explorer {
public:
    explicit Explorer(const Folder& _rootFolder);

    std::vector<File> findTestCases() const;

private:
    static bool isTestCase(const File& file);

    Folder rootFolder;
};

std::vector<File> explore();

}

#endif
