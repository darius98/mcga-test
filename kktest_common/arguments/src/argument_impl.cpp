#include <iostream>

#include "argument_impl.hpp"

using kktest::strutil::toLower;
using std::cout;
using std::invalid_argument;

namespace kktest {
namespace arguments {

ArgumentImpl::ArgumentImpl(const String& _defaultValue, String _implicitValue):
        GenericArgumentImpl<String>(_defaultValue, _implicitValue) {}

ArgumentImpl::~ArgumentImpl() = default;

void ArgumentImpl::setValue(const String& _value) {
    value = _value;
}

FlagImpl::FlagImpl(): GenericArgumentImpl<bool>(false, true) {}

FlagImpl::~FlagImpl() = default;

void FlagImpl::setValue(const String& _value) {
    value = toLower(_value) == "true" ||
            _value == "1" ||
            toLower(_value) == "enabled";
}

IntArgumentImpl::IntArgumentImpl(int defaultValue, int implicitValue):
        GenericArgumentImpl<int>(defaultValue, implicitValue) {}

IntArgumentImpl::~IntArgumentImpl() = default;

void IntArgumentImpl::setValue(const String& _value) {
    try {
        value = stoi(_value);
    } catch(const invalid_argument& error) {
        cout << "Invalid value for int argument: \"" << _value << "\"\n";
        exit(1);
    }
}

}  // namespace arguments
}  // namespace kktest
