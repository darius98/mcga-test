#include "common/interproc/src/pipe.hpp"

using namespace std;

namespace kktest::interproc {

Message PipeReader::getNextMessage() {
    return getNextMessage(-1);
}

vector<uint8_t> PipeReader::getBytes() {
    return getBytes(1);
}

size_t PipeReader::GetMessageSize(const Message& message) {
    return message.getSize();
}

void PipeWriter::sendMessage(const Message& message) {
    sendBytes(message.payload, message.getSize());
}

}
