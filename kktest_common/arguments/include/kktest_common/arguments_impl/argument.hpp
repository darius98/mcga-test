#ifndef KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENT_HPP_
#define KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENT_HPP_

#include <kktest_common/strutil.hpp>

namespace kktest {
namespace arguments {

class Argument {
 public:
    virtual ~Argument() = default;

    virtual String get() const = 0;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENT_HPP_
