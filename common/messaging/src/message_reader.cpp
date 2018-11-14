#include "message_reader.hpp"

using namespace std;

namespace messaging {

MessageReader::MessageReader(const Message& _message): message(_message) {}

MessageReader& MessageReader::operator<<(string& obj) {
    string::size_type size;
    (*this) << size;
    obj = string((char*)message.getPayload() + cursor, (char*)message.getPayload() + cursor + size);
    cursor += size;
    return *this;
}

}
