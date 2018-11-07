#include <alloca.h>
#include <unistd.h>

#include "pipe.hpp"

using namespace std;

namespace kktest {

Pipe::Pipe(const int& _outputFD): outputFD(_outputFD) {}

void Pipe::pipe(const MessageSerializable* messageSerializable) const {
    Message message = messageSerializable->toMessage();
    writeBytes(message.getPayload(), message.getSize());
}

void Pipe::writeBytes(const void* bytes, size_t numBytes) const {
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

}
