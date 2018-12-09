#include <iostream>
#include <stdexcept>

#include "int_argument_impl.hpp"

using std::cout;
using std::invalid_argument;

namespace kktest {
namespace arguments {

IntArgumentImpl::IntArgumentImpl(int _defaultValue, int _implicitValue):
        value(_defaultValue),
        defaultValue(_defaultValue),
        implicitValue(_implicitValue) {}

IntArgumentImpl::~IntArgumentImpl() = default;

int IntArgumentImpl::get() const {
    return value;
}

void IntArgumentImpl::setDefault() {
    value = defaultValue;
}

void IntArgumentImpl::setImplicit() {
    value = implicitValue;
}

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
