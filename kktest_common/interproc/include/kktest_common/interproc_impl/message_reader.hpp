#ifndef KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_READER_HPP_
#define KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_READER_HPP_

#include <kktest_common/interproc_impl/message.hpp>

namespace kktest {
namespace interproc {

class MessageReader {
 public:
    explicit MessageReader(const Message& _message);

    MessageReader& operator<<(String& obj);

    template<class T>
    MessageReader& operator<<(T& obj) {
        obj = *static_cast<T*>(
                static_cast<void*>(
                  static_cast<std::uint8_t*>(message.raw()) + cursor));
        cursor += sizeof(obj);
        return *this;
    }

 private:
    int cursor = Message::METADATA_SIZE;
    const Message& message;
};

}  // namespace interproc
}  // namespace kktest

#endif  // KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_MESSAGE_READER_HPP_
