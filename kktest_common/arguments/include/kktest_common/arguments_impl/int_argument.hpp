#ifndef KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_INT_ARGUMENT_HPP_
#define KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_INT_ARGUMENT_HPP_

namespace kktest {
namespace arguments {

class IntArgument {
 public:
    virtual ~IntArgument() = default;

    virtual int get() const = 0;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_INT_ARGUMENT_HPP_
