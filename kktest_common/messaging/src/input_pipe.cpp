#include <unistd.h>

#include <cstring>

#include <kktest_common/messaging_impl/input_pipe.hpp>

using std::size_t;

namespace kktest {
namespace messaging {

InputPipe::InputPipe(const int& _inputFD):
        inputFD(_inputFD),
        buffer(malloc(128)),
        bufferSize(0),
        bufferCapacity(128) {}

InputPipe::~InputPipe() {
    free(buffer);
}

Message InputPipe::getNextMessage(int maxConsecutiveFailedReadAttempts) {
    char block[128];
    size_t messageSize = 0;
    int failedAttempts = 0;
    while (failedAttempts <= maxConsecutiveFailedReadAttempts) {
        ssize_t numBytesRead = read(inputFD, block, 128);
        if (numBytesRead < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                failedAttempts += 1;
                continue;
            }
            perror("read");
            exit(errno);
        }
        if (numBytesRead == 0) {
            failedAttempts += 1;
        } else {
            failedAttempts = 0;
            resizeBufferToFit((size_t)numBytesRead);
            memcpy(static_cast<uint8_t*>(buffer) + bufferSize,
                   block,
                   static_cast<size_t>(numBytesRead));
            bufferSize += numBytesRead;
        }
        messageSize = Message::isSane(buffer, bufferSize);
        if (messageSize != 0) {
            break;
        }
    }
    if (!messageSize) {
        return Message(nullptr);
    }
    return extractMessageFromBuffer(messageSize);
}

void InputPipe::close() {
    int ret = ::close(inputFD);
    if (ret < 0) {
        perror("close");
        exit(errno);
    }
}

void InputPipe::resizeBufferToFit(size_t extraBytes) {
    while (bufferCapacity < bufferSize + extraBytes) {
        void* newBuffer = malloc(2 * bufferCapacity);
        memcpy(newBuffer, buffer, bufferSize);
        free(buffer);
        buffer = newBuffer;
        bufferCapacity *= 2;
    }
}

Message InputPipe::extractMessageFromBuffer(size_t messageSize) {
    void* messagePayload = malloc(messageSize);
    memcpy(messagePayload, buffer, messageSize);
    memcpy(buffer, static_cast<uint8_t*>(buffer) + messageSize, bufferSize - messageSize);
    bufferSize -= messageSize;
    return Message(messagePayload);
}

}  // namespace messaging
}  // namespace kktest
