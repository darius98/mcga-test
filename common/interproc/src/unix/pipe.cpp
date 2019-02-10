#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <kktest_common/interproc_impl/pipe.hpp>

using namespace std;

namespace kktest {
namespace interproc {

class LinuxPipeReader: public PipeReader {
 public:
    explicit LinuxPipeReader(const int& _inputFD):
        inputFD(_inputFD),
        buffer(malloc(128)),
        bufferReadHead(0),
        bufferSize(0),
        bufferCapacity(128) {}

    ~LinuxPipeReader() override {
        close();
        free(buffer);
    }

    Message getNextMessage(int maxConsecutiveFailedReadAttempts) override {
        // Try reading a message first, maybe we received multiple at once.
        auto message = readMessageFromBuffer();
        if (!message.isInvalid()) {
            return message;
        }

        char block[128];
        int failedAttempts = 0;
        while (maxConsecutiveFailedReadAttempts == -1 ||
               failedAttempts <= maxConsecutiveFailedReadAttempts) {
            ssize_t numBytesRead = read(inputFD, block, 128);
            if (numBytesRead < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    failedAttempts += 1;
                    continue;
                }
                // TODO(darius98): Handle errors better than just exiting!
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
            message = readMessageFromBuffer();
            if (!message.isInvalid()) {
                return message;
            }
        }
        return Message::INVALID;
    }

 private:
    void close() {
        if (closed) {
            return;
        }
        int ret = ::close(inputFD);
        if (ret < 0) {
            // TODO(darius98): Handle errors better than just exiting!
            perror("close");
            exit(errno);
        }
        closed = true;
    }

    void resizeBufferToFit(std::size_t extraBytes) {
        if (bufferCapacity < bufferSize + extraBytes && bufferReadHead > 0) {
            memcpy(buffer,
                   static_cast<uint8_t*>(buffer) + bufferReadHead,
                   bufferCapacity - bufferReadHead);
            bufferSize -= bufferReadHead;
            bufferReadHead = 0;
        }
        while (bufferCapacity < bufferSize + extraBytes) {
            void* newBuffer = malloc(2 * bufferCapacity);
            memcpy(newBuffer, buffer, bufferSize);
            free(buffer);
            buffer = newBuffer;
            bufferCapacity *= 2;
        }
    }

    Message readMessageFromBuffer() {
        auto message = Message::read(
                static_cast<uint8_t*>(buffer) + bufferReadHead,
                bufferSize - bufferReadHead);
        if (!message.isInvalid()) {
            bufferReadHead += message.getSize();
        }
        return message;
    }

    bool closed = false;
    int inputFD;
    void* buffer;
    size_t bufferReadHead;
    size_t bufferSize;
    size_t bufferCapacity;
};

class LinuxPipeWriter: public PipeWriter {
 public:
    explicit LinuxPipeWriter(const int& _outputFD):
        outputFD(_outputFD) {}

    ~LinuxPipeWriter() override {
        close();
    }

 private:
    void close() {
        if (closed) {
            return;
        }
        int ret = ::close(outputFD);
        if (ret < 0) {
            // TODO(darius98): Handle errors better than just exiting!
            perror("close");
            exit(errno);
        }
        closed = true;
    }

    void sendBytes(void* bytes, size_t numBytes) override {
        size_t written = 0;
        while (written < numBytes) {
            auto target = static_cast<const uint8_t*>(bytes) + written;
            size_t remaining = numBytes - written;
            ssize_t currentWriteBlockSize = write(outputFD, target, remaining);
            if (currentWriteBlockSize < 0) {
                // TODO(darius98): Handle errors better than just exiting!
                perror("write");
                exit(errno);
            }
            written += currentWriteBlockSize;
        }
    }

    bool closed = false;
    int outputFD;
};

pair<PipeReader*, PipeWriter*> createAnonymousPipe() {
    int fd[2];
    if (pipe(fd) < 0) {
        // TODO(darius98): Handle errors better than just exiting!
        perror("pipe");
        exit(errno);
    }
    fcntl(fd[0], F_SETFL, O_NONBLOCK);
    fcntl(fd[1], F_SETFL, O_NONBLOCK);
    return {new LinuxPipeReader(fd[0]), new LinuxPipeWriter(fd[1])};
}

void createNamedPipe(const String& pipeName) {
    int pipeCreateStatus = mkfifo(pipeName.c_str(), 0666);
    if (pipeCreateStatus < 0) {
        // TODO(darius98): Handle errors better than just exiting!
        perror("mkfifo");
        exit(errno);
    }
}

void destroyNamedPipe(const String& pipeName) {
    int removeStat = remove(pipeName.c_str());
    if (removeStat < 0) {
        perror("remove pipe");
        exit(errno);
    }
}

PipeReader* openNamedPipeForReading(const String& pipeName) {
    int pipeFD = open(pipeName.c_str(), O_RDONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        // TODO(darius98): Handle errors better than just exiting!
        perror("open");
        exit(errno);
    }
    return new LinuxPipeReader(pipeFD);
}

PipeWriter* openNamedPipeForWriting(const String& pipeName) {
    int pipeFD = open(pipeName.c_str(), O_WRONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        // TODO(darius98): Handle errors better than just exiting!
        perror("open");
        exit(errno);
    }
    return new LinuxPipeWriter(pipeFD);
}

}
}
