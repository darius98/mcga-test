#ifndef COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_MESSAGE_READER_HPP_
#define COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_MESSAGE_READER_HPP_

#include <string>

#include <messaging_impl/message.hpp>

namespace messaging {

class MessageReader {
 public:
    explicit MessageReader(const Message& _message);

    MessageReader& operator<<(std::string& obj);

    template<class T>
    MessageReader& operator<<(T& obj) {
        void* objAddress = static_cast<std::uint8_t*>(message.getPayload()) + cursor;
        obj = *static_cast<T*>(objAddress);
        cursor += sizeof(obj);
        return *this;
    }

 private:
    int cursor = sizeof(std::size_t);
    const Message& message;
};

}  // namespace messaging

#endif  // COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_MESSAGE_READER_HPP_
