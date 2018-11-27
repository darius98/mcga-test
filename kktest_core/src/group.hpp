#ifndef KKTEST_CORE_GROUP_H_
#define KKTEST_CORE_GROUP_H_

#include <string>

#include <kktest_impl/executable.hpp>
#include <kktest_impl/info.hpp>
#include "test.hpp"

namespace kktest {

class Group {
public:
    Group(const GroupConfig& _config, Group* _parentGroup, int _index);

    std::string getFileAndLine() const;

    int getIndex() const;

    GroupInfo getGroupInfo() const;

    Group* getParentGroup() const;

    void addSetUp(Executable func, const std::string& file, int line);

    void setUp() const;

    void addTearDown(Executable func, const std::string& file, int line);

    void tearDown() const;

    std::string getRenderedFailureMessageOnExceptionInSetUp(const std::string& what) const;

    std::string getRenderedFailureMessageOnNonExceptionInSetUp() const;

    std::string getRenderedFailureMessageOnExceptionInTearDown(const std::string& what) const;

    std::string getRenderedFailureMessageOnNonExceptionInTearDown() const;

private:
    GroupConfig config;

    Group* parentGroup;
    int index;

    bool hasSetUp = false;
    CopyableExecutable setUpFunc;
    std::string setUpFile;
    int setUpLine;

    bool hasTearDown = false;
    CopyableExecutable tearDownFunc;
    std::string tearDownFile;
    int tearDownLine;
};

}

#endif
