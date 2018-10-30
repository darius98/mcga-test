#ifndef KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_
#define KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_

#include <core/group.hpp>
#include <core/test.hpp>

namespace kktest {

class Pipe {
public:
    explicit Pipe(const int& _outputFD);

    void pipeTest(Test* test) const;

    void pipeGroup(Group* group) const;

private:
    void writeBytes(const uint8_t* bytes, const std::size_t& numBytes) const;

    int outputFD;
};

}

#endif
