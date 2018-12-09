#ifndef KKTEST_COMMON_ARGUMENTS_SRC_INT_ARGUMENT_IMPL_HPP_
#define KKTEST_COMMON_ARGUMENTS_SRC_INT_ARGUMENT_IMPL_HPP_

#include <kktest_common/arguments_impl/int_argument.hpp>
#include "command_line_spec.hpp"

namespace kktest {
namespace arguments {

class IntArgumentImpl: public IntArgument, public CommandLineSpec {
 public:
    IntArgumentImpl(int _defaultValue, int _implicitValue);

    ~IntArgumentImpl() override;

    int get() const override;

    void setDefault() override;

    void setImplicit() override;

    void setValue(const String& _value) override;

 private:
    int value;
    int defaultValue;
    int implicitValue;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_SRC_INT_ARGUMENT_IMPL_HPP_
