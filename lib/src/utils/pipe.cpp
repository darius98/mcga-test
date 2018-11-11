#include <unistd.h>

#include <cstring>

#include "pipe.hpp"

using namespace std;

namespace kktest {

OutputPipe::OutputPipe(const int& _outputFD): outputFD(_outputFD) {}

void OutputPipe::pipe(const Message& message) const {
    const void* bytes = message.getPayload();
    size_t numBytes = message.getSize();
    size_t written = 0;
    while (written < numBytes) {
        uint8_t* target = ((uint8_t*)bytes) + written;
        size_t remaining = numBytes - written;
        ssize_t currentWriteBlockSize = write(outputFD, target, remaining);
        if (currentWriteBlockSize < 0) {
            perror("write");
            exit(errno);
        }
        written += currentWriteBlockSize;
    }
}

InputPipe::InputPipe(const int& _inputFD):
        inputFD(_inputFD), buffer(malloc(128)), bufferSize(0), bufferCapacity(128) {}

InputPipe::~InputPipe() {
    free(buffer);
}

Message InputPipe::getNextMessage(int readAttempts) {
    char block[128];
    size_t messageSize;
    for (int i = 0; i < readAttempts; ++ i) {
        ssize_t numBytesRead = read(inputFD, block, 128);
        if (numBytesRead < 0) {
            perror("read");
            exit(errno);
        }
        resizeBufferToFit(numBytesRead);
        memcpy((uint8_t*)buffer + bufferSize, block, numBytesRead);
        bufferSize += numBytesRead;
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
    memcpy(buffer, (uint8_t*)buffer + messageSize, bufferSize - messageSize);
    bufferSize -= messageSize;
    return Message(messagePayload);
}

}
