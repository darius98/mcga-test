#include <unistd.h>

#include <cstring>

#include <kktest_common/messaging_impl/output_pipe.hpp>

using std::size_t;

namespace kktest {
namespace messaging {

OutputPipe::OutputPipe(const int& _outputFD): outputFD(_outputFD) {}

void OutputPipe::close() {
    int ret = ::close(outputFD);
    if (ret < 0) {
        perror("close");
        exit(errno);
    }
}

void OutputPipe::pipe(const Message& message) const {
    const void* bytes = message.getPayload();
    size_t numBytes = message.getSize();
    size_t written = 0;
    while (written < numBytes) {
        auto target = static_cast<const uint8_t*>(bytes) + written;
        size_t remaining = numBytes - written;
        ssize_t currentWriteBlockSize = write(outputFD, target, remaining);
        if (currentWriteBlockSize < 0) {
            perror("write");
            exit(errno);
        }
        written += currentWriteBlockSize;
    }
}

}  // namespace messaging
}  // namespace kktest
