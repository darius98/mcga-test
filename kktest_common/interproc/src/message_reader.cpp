#include <kktest_common/interproc_impl/message_reader.hpp>

namespace kktest {
namespace interproc {

MessageReader::MessageReader(const Message& _message): message(_message) {}

MessageReader& MessageReader::operator<<(String& obj) {
    decltype(obj.size()) size;
    (*this) << size;
    obj.assign(static_cast<char*>(message.raw()) + cursor,
               static_cast<char*>(message.raw()) + cursor + size);
    cursor += size;
    return *this;
}

}  // namespace interproc
}  // namespace kktest
