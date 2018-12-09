#ifndef KKTEST_COMMON_ARGUMENTS_SRC_COMMAND_LINE_SPEC_HPP_
#define KKTEST_COMMON_ARGUMENTS_SRC_COMMAND_LINE_SPEC_HPP_

#include <kktest_common/strutil.hpp>

namespace kktest {
namespace arguments {

class CommandLineSpec {
 public:
    virtual ~CommandLineSpec() = default;

    virtual void setDefault() = 0;

    virtual void setImplicit() = 0;

    virtual void setValue(const String& _value) = 0;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_SRC_COMMAND_LINE_SPEC_HPP_
