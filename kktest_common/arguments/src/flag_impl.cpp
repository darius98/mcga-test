#include "flag_impl.hpp"

using kktest::strutil::toLower;

namespace kktest {
namespace arguments {

FlagImpl::~FlagImpl() = default;

bool FlagImpl::get() const {
    return value;
}

void FlagImpl::setDefault() {
    value = false;
}

void FlagImpl::setImplicit() {
    value = true;
}

void FlagImpl::setValue(const String& _value) {
    value = toLower(_value) == "true" || _value == "1" || toLower(_value) == "enabled";
}

}  // namespace arguments
}  // namespace kktest
