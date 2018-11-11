#ifndef KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_
#define KKTEST_PLUGINS_PIPE_PLUGIN_PIPE_H_

#include <utils/message.hpp>


namespace kktest {

class OutputPipe {
public:
    explicit OutputPipe(const int& _outputFD);

    void pipe(const Message& message) const;

private:
    int outputFD;
};

class InputPipe {
public:
    explicit InputPipe(const int& _inputFD);

    ~InputPipe();

    Message getNextMessage(int readAttempts=1024);

private:
    int inputFD;
    void* buffer;
    int bufferSize;
    int bufferCapacity;
};

}

#endif
