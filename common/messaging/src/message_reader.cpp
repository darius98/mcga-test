#include <messaging_impl/message_reader.hpp>

using std::string;

namespace messaging {

MessageReader::MessageReader(const Message& _message): message(_message) {}

MessageReader& MessageReader::operator<<(string& obj) {
    string::size_type size;
    (*this) << size;
    obj.assign(static_cast<char*>(message.getPayload()) + cursor,
               static_cast<char*>(message.getPayload()) + cursor + size);
    cursor += size;
    return *this;
}

}  // namespace messaging
