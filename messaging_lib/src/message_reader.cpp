#include "message_reader.hpp"

using namespace std;

namespace messaging {

MessageReader::MessageReader(const Message& _message): message(_message) {}

template<>
string MessageReader::read() {
    auto size = read<string::size_type>();
    string ans((char*)message.getPayload() + cursor, (char*)message.getPayload() + cursor + size);
    cursor += size;
    return ans;
}

}
