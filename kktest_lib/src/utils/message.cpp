#include <cstdint>
#include <cstring>

#include "message.hpp"

using namespace std;

namespace kktest {

Message::Message(size_t size): payload(malloc(size + sizeof(size_t))) {
    memcpy(payload, &size, sizeof(size_t));
    cursor = sizeof(size_t);
}

Message::Message(const Message& other): cursor(other.cursor) {
    size_t size = *((size_t*)other.payload);
    payload = malloc(size + sizeof(size_t));
    memcpy(payload, other.payload, cursor);
}

Message::Message(Message&& other) noexcept:
        cursor(other.cursor), payload(other.payload) {
    other.payload = nullptr;
}

Message::~Message() {
    free(payload);
}

Message& Message::addBytes(void* bytes, size_t numBytes) {
    memcpy((uint8_t*)payload + cursor, bytes, numBytes);
    cursor += numBytes;
    return *this;
}

Message MessageSerializable::toMessage() const {
    Message message(numBytes());
    serialize(message);
    return message;
}

}
