#include <alloca.h>
#include <unistd.h>

#include "test_pipe.hpp"

using namespace std;

namespace kktest {

TestPipe::TestPipe(const int& _outputFD): outputFD(_outputFD) {}

void TestPipe::pipeTest(Test* test) const {
    size_t testSize = test->numBytes();
    writeBytes((uint8_t*)&testSize, sizeof(size_t));
    uint8_t* serializedTest = (uint8_t*)alloca(testSize);
    test->writeBytes(serializedTest);
    writeBytes(serializedTest, testSize);
}

void TestPipe::pipeGroup(Group* group) const {
    size_t groupSize = group->numBytes();
    writeBytes((uint8_t*)&groupSize, sizeof(size_t));
    uint8_t* serializedGroup = (uint8_t*)alloca(groupSize);
    group->writeBytes(serializedGroup);
    writeBytes(serializedGroup, groupSize);
}

void TestPipe::writeBytes(const uint8_t* bytes, const size_t& numBytes) const {
    size_t written = 0;
    while (written < numBytes) {
        size_t cWritten = write(outputFD, bytes + written, numBytes - written);
        if (cWritten < 0) {
            perror("write");
            exit(errno);
        }
        written += cWritten;
    }
}

}
