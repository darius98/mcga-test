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

template<class T>
struct GenericArgumentBuilder {
    GenericArgumentBuilder(String _name, String _helpText):
            name(std::move(_name)), helpText(std::move(_helpText)) {}

    GenericArgumentBuilder& withShortName(const String& _shortName) {
        shortName = _shortName;
        return *this;
    }

    GenericArgumentBuilder& withDefaultValue(const T& _defaultValue) {
        defaultValue = _defaultValue;
        return *this;
    }

    GenericArgumentBuilder& withImplicitValue(const T& _implicitValue) {
        implicitValue = _implicitValue;
        return *this;
    }

    String name;
    String helpText;
    String shortName;
    T defaultValue;
    T implicitValue;
};

typedef GenericArgumentBuilder<String> ArgumentBuilder;
typedef GenericArgumentBuilder<bool> FlagBuilder;
typedef GenericArgumentBuilder<int> IntArgumentBuilder;

}  // namespace arguments
}  // namespace kktest

#endif  // KKTEST_COMMON_ARGUMENTS_INCLUDE_KKTEST_COMMON_ARGUMENTS_IMPL_ARGUMENT_HPP_