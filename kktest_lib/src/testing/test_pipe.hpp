#ifndef KKTEST_TESTING_TEST_PIPE_H_
#define KKTEST_TESTING_TEST_PIPE_H_

#include "group.hpp"
#include "test.hpp"

namespace kktest {

class TestPipe {
public:
    explicit TestPipe(const int& _outputFD);

    void pipeTest(Test* test) const;

    void pipeGroup(Group* group) const;

private:
    void writeBytes(const uint8_t* bytes, const std::size_t& numBytes) const;

    int outputFD;
};

}

#endif
