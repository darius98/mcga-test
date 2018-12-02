#ifndef KKTEST_COMMON_MESSAGING_INCLUDE_KKTEST_COMMON_MESSAGING_IMPL_INPUT_PIPE_HPP_
#define KKTEST_COMMON_MESSAGING_INCLUDE_KKTEST_COMMON_MESSAGING_IMPL_INPUT_PIPE_HPP_

#include <kktest_common/messaging_impl/message.hpp>

namespace kktest {
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
}  // namespace kktest

#endif  // KKTEST_COMMON_MESSAGING_INCLUDE_KKTEST_COMMON_MESSAGING_IMPL_INPUT_PIPE_HPP_
