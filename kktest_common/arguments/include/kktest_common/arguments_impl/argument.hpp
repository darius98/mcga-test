#ifndef KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENT_HPP_
#define KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENT_HPP_

#include <kktest_common/strutil.hpp>

namespace kktest {
namespace arguments {

template<class T>
class GenericArgument {
 public:
    virtual ~GenericArgument() = default;

    virtual T get() const = 0;
};

typedef GenericArgument<String> Argument;

typedef GenericArgument<bool> Flag;

typedef GenericArgument<int> IntArgument;

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENT_HPP_
