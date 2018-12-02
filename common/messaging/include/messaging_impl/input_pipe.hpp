#ifndef COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_INPUT_PIPE_HPP_
#define COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_INPUT_PIPE_HPP_

#include <messaging_impl/message.hpp>

namespace messaging {

class InputPipe {
 public:
    explicit InputPipe(const int& _inputFD);

    ~InputPipe();

    void close();

    Message getNextMessage(int maxConsecutiveFailedReadAttempts = 32);

 private:
    void resizeBufferToFit(std::size_t extraBytes);

    Message extractMessageFromBuffer(std::size_t messageSize);

    int inputFD;
    void* buffer;
    std::size_t bufferSize;
    std::size_t bufferCapacity;
};

}  // namespace messaging

#endif  // COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_INPUT_PIPE_HPP_
