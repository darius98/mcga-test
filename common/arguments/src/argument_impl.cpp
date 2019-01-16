#include <iostream>

#include "argument_impl.hpp"

using namespace std;

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
    value = _value.toLower() == "true"
            || _value == "1"
            || _value.toLower() == "enabled";
}

IntArgumentImpl::IntArgumentImpl(int defaultValue, int implicitValue):
        GenericArgumentImpl<int>(defaultValue, implicitValue) {}

IntArgumentImpl::~IntArgumentImpl() = default;

void IntArgumentImpl::setValue(const String& _value) {
    value = stoi(_value);
}

}
}
