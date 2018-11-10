#include <cstdint>
#include <cstring>

#include "message.hpp"

using namespace std;

namespace kktest {

BytesConsumer& BytesConsumer::add(const string& str) {
    addBytes(str.c_str(), str.size());
    return *this;
}

BytesCounter& BytesCounter::addBytes(const void*, size_t numBytes) {
    bytesConsumed += numBytes;
    return *this;
}

size_t BytesCounter::getNumBytesConsumed() const {
    return bytesConsumed;
}

Message::Message(size_t size): payload(malloc(size + sizeof(size_t))) {
    memcpy(payload, &size, sizeof(size_t));
    cursor = sizeof(size_t);
}

Message::Message(const Message& other) {
    size_t size = *((size_t*)other.payload);
    payload = malloc(size + sizeof(size_t));
    memcpy(payload, other.payload, cursor);
    cursor = other.cursor;
}

Message::Message(Message&& other) noexcept: payload(other.payload), cursor(other.cursor) {
    other.payload = nullptr;
}

Message::~Message() {
    free(payload);
}

Message& Message::addBytes(const void* bytes, size_t numBytes) {
    memcpy((uint8_t*)payload + cursor, bytes, numBytes);
    cursor += numBytes;
    return *this;
}

void* Message::getPayload() const {
    return payload;
}

size_t Message::getSize() const {
    return *((size_t*)payload) + sizeof(size_t);
}

MessageSerializable::~MessageSerializable() = default;

Message MessageSerializable::toMessage() const {
    BytesCounter counter;
    writeBytes(counter);
    Message message(counter.getNumBytesConsumed());
    writeBytes(message);
    return message;
}

}
