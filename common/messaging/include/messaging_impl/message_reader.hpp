#ifndef COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_MESSAGE_READER_H_
#define COMMON_MESSAGING_INCLUDE_MESSAGING_IMPL_MESSAGE_READER_H_

#include <messaging_impl/message.hpp>

namespace messaging {

class MessageReader {
public:
    explicit MessageReader(const Message& _message);

    MessageReader& operator<<(std::string& obj);

    template<class T>
    MessageReader& operator<<(T& obj) {
        obj = *((T*)((std::uint8_t*)message.getPayload() + cursor));
        cursor += sizeof(T);
        return *this;
    }

private:
    int cursor = sizeof(std::size_t);
    const Message& message;
};

}

#endif
