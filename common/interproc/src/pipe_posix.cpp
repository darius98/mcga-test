#include "common/interproc/src/pipe.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <system_error>

using namespace std;

namespace kktest::interproc {

class PosixPipeReader: public PipeReader {
 public:
    explicit PosixPipeReader(const int& inputFD):
        inputFD(inputFD),
        buffer(static_cast<uint8_t*>(malloc(128))),
        bufferReadHead(0),
        bufferSize(0),
        bufferCapacity(128) {}

    ~PosixPipeReader() override {
        close(inputFD);
        free(buffer);
    }

    Message getNextMessage(int maxConsecutiveFailedReadAttempts) override {
        // Try reading a message first, maybe we received multiple at once.
        auto message = readMessageFromBuffer();
        if (!message.isInvalid()) {
            return message;
        }

        int failedAttempts = 0;
        while (maxConsecutiveFailedReadAttempts == -1 ||
               failedAttempts <= maxConsecutiveFailedReadAttempts) {
            bool successful = readBytes();
            if (!successful) {
                failedAttempts += 1;
                continue;
            }
            failedAttempts = 0;
            message = readMessageFromBuffer();
            if (!message.isInvalid()) {
                return message;
            }
        }
        return Message::INVALID;
    }

    vector<uint8_t> getBytes(int maxConsecutiveFailedReadAttempts) override {
        int failedAttempts = 0;
        while (failedAttempts <= maxConsecutiveFailedReadAttempts) {
            bool successful = readBytes();
            if (!successful) {
                failedAttempts += 1;
            } else {
                failedAttempts = 0;
            }
        }
        vector<uint8_t> result(bufferSize - bufferReadHead, 0);
        memcpy(result.data(),
               buffer + bufferReadHead,
               bufferSize - bufferReadHead);
        bufferReadHead = bufferSize;
        return result;
    }

 private:
    bool readBytes() {
        char block[128];
        ssize_t numBytesRead = read(inputFD, block, 128);
        if (numBytesRead < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                return false;
            }
            throw system_error(errno, generic_category(),
                               "PipeReader:readBytes()");
        }
        if (numBytesRead == 0) {
            return false;
        }
        resizeBufferToFit((size_t)numBytesRead);
        memcpy(buffer + bufferSize, block, static_cast<size_t>(numBytesRead));
        bufferSize += numBytesRead;
        return true;
    }

    void resizeBufferToFit(size_t extraBytes) {
        if (bufferCapacity < bufferSize + extraBytes && bufferReadHead > 0) {
            memcpy(buffer,
                   buffer + bufferReadHead,
                   bufferCapacity - bufferReadHead);
            bufferSize -= bufferReadHead;
            bufferReadHead = 0;
        }
        while (bufferCapacity < bufferSize + extraBytes) {
            auto newBuffer = static_cast<uint8_t*>(malloc(2 * bufferCapacity));
            memcpy(newBuffer, buffer, bufferSize);
            free(buffer);
            buffer = newBuffer;
            bufferCapacity *= 2;
        }
    }

    Message readMessageFromBuffer() {
        auto message = Message::Read(buffer + bufferReadHead,
                                     bufferSize - bufferReadHead);
        if (!message.isInvalid()) {
            bufferReadHead += GetMessageSize(message);
        }
        return message;
    }

    int inputFD;
    uint8_t* buffer;
    size_t bufferReadHead;
    size_t bufferSize;
    size_t bufferCapacity;
};

class PosixPipeWriter: public PipeWriter {
 public:
    explicit PosixPipeWriter(const int& outputFD): outputFD(outputFD) {}

    ~PosixPipeWriter() override {
        close(outputFD);
    }

    void sendBytes(uint8_t* bytes, size_t numBytes) override {
        size_t written = 0;
        while (written < numBytes) {
            auto target = bytes + written;
            size_t remaining = numBytes - written;
            ssize_t currentWriteBlockSize = write(outputFD, target, remaining);
            if (currentWriteBlockSize < 0) {
                throw system_error(errno, generic_category(),
                                   "PipeWriter:sendBytes");
            }
            written += currentWriteBlockSize;
        }
    }

    int outputFD;
};

void redirectStdoutToPipe(PipeWriter* pipeWriter) {
    auto linuxPipeWriter = dynamic_cast<PosixPipeWriter*>(pipeWriter);
    int ret = dup2(linuxPipeWriter->outputFD, STDOUT_FILENO);
    if (ret < 0) {
        throw system_error(errno, generic_category(),
                           "redirectStdoutToPipe:dup2");
    }
    ret = ::close(linuxPipeWriter->outputFD);
    if (ret < 0) {
        throw system_error(errno, generic_category(),
                           "redirectStdoutToPipe:closing initial pipe");
    }
    linuxPipeWriter->outputFD = STDOUT_FILENO;
}

pair<PipeReader*, PipeWriter*> createAnonymousPipe() {
    int fd[2];
    if (pipe(fd) < 0) {
        throw system_error(errno, generic_category(),
                           "createAnonymousPipe:pipe");
    }
    if (fcntl(fd[0], F_SETFL, O_NONBLOCK) < 0) {
        throw system_error(errno, generic_category(),
                           "createAnonymousPipe:fcntl (set read non-blocking");
    }
    if (fcntl(fd[1], F_SETFL, O_NONBLOCK) < 0) {
        throw system_error(errno, generic_category(),
                           "createAnonymousPipe:fcntl (set write non-blocking");
    }
    return {new PosixPipeReader(fd[0]), new PosixPipeWriter(fd[1])};
}

void createNamedPipe(const string& pipeName) {
    int pipeCreateStatus = mkfifo(pipeName.c_str(), 0666);
    if (pipeCreateStatus < 0) {
        throw system_error(errno, generic_category(), "createNamedPipe:mkfifo");
    }
}

void destroyNamedPipe(const string& pipeName) {
    int removeStat = remove(pipeName.c_str());
    if (removeStat < 0) {
        throw system_error(errno, generic_category(),
                           "destroyNamedPipe:remove");
    }
}

PipeReader* openNamedPipeForReading(const string& pipeName) {
    int pipeFD = open(pipeName.c_str(), O_RDONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        throw system_error(errno, generic_category(),
                           "openNamedPipeForReading:open");
    }
    return new PosixPipeReader(pipeFD);
}

PipeWriter* openNamedPipeForWriting(const string& pipeName) {
    int pipeFD = open(pipeName.c_str(), O_WRONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        throw system_error(errno, generic_category(),
                           "openNamedPipeForWriting:open");
    }
    return new PosixPipeWriter(pipeFD);
}

}
