#ifndef MESSAGING_LIB_INCLUDE_INPUT_PIPE_H_
#define MESSAGING_LIB_INCLUDE_INPUT_PIPE_H_

#include "message.hpp"


namespace messaging {

class InputPipe {
public:
    explicit InputPipe(const int& _inputFD);

    ~InputPipe();

    Message getNextMessage(int readAttempts=1024);

private:
    void resizeBufferToFit(std::size_t extraBytes);

    Message extractMessageFromBuffer(std::size_t messageSize);

    int inputFD;
    void* buffer;
    std::size_t bufferSize;
    std::size_t bufferCapacity;
};

}

#endif
