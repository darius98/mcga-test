#ifndef KKTEST_COMMON_ARGUMENTS_ARGUMENT_IMPL_HPP_
#define KKTEST_COMMON_ARGUMENTS_ARGUMENT_IMPL_HPP_

#include <kktest_common/arguments_impl/argument.hpp>
#include <kktest_common/string.hpp>
#include "command_line_spec.hpp"

namespace kktest {
namespace arguments {

template<class T>
class GenericArgumentImpl: public GenericArgument<T>, public CommandLineSpec {
 public:
    GenericArgumentImpl(const T& _defaultValue, const T& _implicitValue):
            value(_defaultValue),
            defaultValue(_defaultValue),
            implicitValue(_implicitValue) {}

    ~GenericArgumentImpl() override = default;

    T get() const override {
        return value;
    }

    void setDefault() override {
        value = defaultValue;
    }

    void setImplicit() override {
        value = implicitValue;
    }

 protected:
    T value;
    T defaultValue;
    T implicitValue;
};

class ArgumentImpl: public GenericArgumentImpl<String> {
 public:
    ArgumentImpl(const String& defaultValue, String implicitValue);

    ~ArgumentImpl() override;

    void setValue(const String& _value) override;
};

class FlagImpl: public GenericArgumentImpl<bool> {
 public:
    FlagImpl();

    ~FlagImpl() override;

    void setValue(const String& _value) override;
};

class IntArgumentImpl: public GenericArgumentImpl<int> {
 public:
    IntArgumentImpl(int defaultValue, int implicitValue);

    ~IntArgumentImpl() override;

    void setValue(const String& _value) override;
};

}
}

#endif
