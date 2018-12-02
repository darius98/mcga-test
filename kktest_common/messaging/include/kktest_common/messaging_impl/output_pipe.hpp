#ifndef KKTEST_COMMON_MESSAGING_INCLUDE_KKTEST_COMMON_MESSAGING_IMPL_OUTPUT_PIPE_HPP_
#define KKTEST_COMMON_MESSAGING_INCLUDE_KKTEST_COMMON_MESSAGING_IMPL_OUTPUT_PIPE_HPP_

#include <kktest_common/messaging_impl/message.hpp>

namespace kktest {
namespace messaging {

class OutputPipe {
 public:
    explicit OutputPipe(const int& _outputFD);

    void close();

    void pipe(const Message& message) const;

 private:
    int outputFD;
};

}  // namespace messaging
}  // namespace kktest

#endif  // KKTEST_COMMON_MESSAGING_INCLUDE_KKTEST_COMMON_MESSAGING_IMPL_OUTPUT_PIPE_HPP_
