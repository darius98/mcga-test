#ifndef KKTEST_COMMON_ARGUMENTS_SRC_ARGUMENT_IMPL_HPP_
#define KKTEST_COMMON_ARGUMENTS_SRC_ARGUMENT_IMPL_HPP_

#include <kktest_common/strutil.hpp>
#include <kktest_common/arguments_impl/argument.hpp>
#include "command_line_spec.hpp"

namespace kktest {
namespace arguments {

class ArgumentImpl: public Argument, public CommandLineSpec {
 public:
    ArgumentImpl(const String& _defaultValue, String _implicitValue);

    ~ArgumentImpl() override;

    String get() const override;

    void setDefault() override;

    void setImplicit() override;

    void setValue(const String& _value) override;

 private:
    String value;
    String defaultValue;
    String implicitValue;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_SRC_ARGUMENT_IMPL_HPP_
