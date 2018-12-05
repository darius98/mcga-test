#include <fcntl.h>
#include <sys/stat.h>
#include <zconf.h>

#include <cstdlib>
#include <cstring>

#include <kktest_common/interproc_impl/pipe.hpp>

using std::pair;
using std::string;

namespace kktest {
namespace interproc {

class LinuxPipeReader: public PipeReader {
 public:
    explicit LinuxPipeReader(const int& _inputFD):
        inputFD(_inputFD),
        buffer(malloc(128)),
        bufferSize(0),
        bufferCapacity(128) {}

    ~LinuxPipeReader() override {
        close();
        free(buffer);
    }

    void close() override {
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

    Message getNextMessage(int maxConsecutiveFailedReadAttempts) override {
        char block[128];
        int failedAttempts = 0;
        while (failedAttempts <= maxConsecutiveFailedReadAttempts) {
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
            auto message = Message::read(buffer, bufferSize);
            if (!message.isInvalid()) {
                size_t size = message.getSize();
                memcpy(buffer, static_cast<uint8_t*>(buffer) + size, bufferSize - size);
                bufferSize -= size;
                return message;
            }
        }
        return Message::invalid();
    }

 private:
    void resizeBufferToFit(std::size_t extraBytes) {
        while (bufferCapacity < bufferSize + extraBytes) {
            void* newBuffer = malloc(2 * bufferCapacity);
            memcpy(newBuffer, buffer, bufferSize);
            free(buffer);
            buffer = newBuffer;
            bufferCapacity *= 2;
        }
    }

    bool closed = false;
    int inputFD;
    void* buffer;
    std::size_t bufferSize;
    std::size_t bufferCapacity;
};

class LinuxPipeWriter: public PipeWriter {
 public:
    explicit LinuxPipeWriter(const int& _outputFD):
        outputFD(_outputFD) {}

    ~LinuxPipeWriter() override {
        close();
    }

    void close() override {
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

    void sendMessage(const Message& message) override {
        const void* bytes = message.getPayload();
        size_t numBytes = message.getSize();
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

 private:
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
    return {new LinuxPipeReader(fd[0]), new LinuxPipeWriter(fd[1])};
}

void createNamedPipe(const string& pipeName) {
    int pipeCreateStatus = mkfifo(pipeName.c_str(), 0666);
    if (pipeCreateStatus < 0) {
        // TODO(darius98): Handle errors better than just exiting!
        perror("mkfifo");
        exit(errno);
    }
}

void destroyNamedPipe(const string& pipeName) {
    int removeStat = remove(pipeName.c_str());
    if (removeStat < 0) {
        perror("remove pipe");
        exit(errno);
    }
}

PipeReader* openNamedPipeForReading(const string& pipeName) {
    int pipeFD = open(pipeName.c_str(), O_RDONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        // TODO(darius98): Handle errors better than just exiting!
        perror("open");
        exit(errno);
    }
    return new LinuxPipeReader(pipeFD);
}

PipeWriter* openNamedPipeForWriting(const string& pipeName) {
    int pipeFD = open(pipeName.c_str(), O_WRONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        // TODO(darius98): Handle errors better than just exiting!
        perror("open");
        exit(errno);
    }
    return new LinuxPipeWriter(pipeFD);
}

}  // namespace interproc
}  // namespace kktest
