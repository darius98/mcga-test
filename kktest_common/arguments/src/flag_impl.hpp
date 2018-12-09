#ifndef KKTEST_COMMON_ARGUMENTS_SRC_FLAG_IMPL_HPP_
#define KKTEST_COMMON_ARGUMENTS_SRC_FLAG_IMPL_HPP_

#include <kktest_common/strutil.hpp>
#include <kktest_common/arguments_impl/flag.hpp>
#include "command_line_spec.hpp"

namespace kktest {
namespace arguments {

class FlagImpl: public Flag, public CommandLineSpec {
 public:
    ~FlagImpl() override;

    bool get() const override;

    void setDefault() override;

    void setImplicit() override;

    void setValue(const String& _value) override;

 private:
    bool value = false;
};

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_SRC_FLAG_IMPL_HPP_
