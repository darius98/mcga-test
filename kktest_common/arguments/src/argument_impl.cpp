#include "argument_impl.hpp"

namespace kktest {
namespace arguments {

ArgumentImpl::ArgumentImpl(const String& _defaultValue, String _implicitValue):
        value(_defaultValue),
        defaultValue(_defaultValue),
        implicitValue(move(_implicitValue)) {}

ArgumentImpl::~ArgumentImpl() = default;

String ArgumentImpl::get() const {
    return value;
}

void ArgumentImpl::setDefault() {
    value = defaultValue;
}

void ArgumentImpl::setImplicit() {
    value = implicitValue;
}

void ArgumentImpl::setValue(const String& _value) {
    value = _value;
}

}  // namespace arguments
}  // namespace kktest
