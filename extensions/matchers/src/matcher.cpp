#include "extensions/matchers/include/kktest_ext/matchers_impl/matcher.hpp"

#include "core/src/export.hpp"

using namespace std;

namespace kktest {
namespace matchers {

KKTEST_EXPORT Description::Description() = default;

KKTEST_EXPORT Description::Description(const Description& other):
        stream(other.stream.str()) {}

KKTEST_EXPORT Description& Description::appendRawString(const string& str) {
    stream << str;
    return *this;
}

KKTEST_EXPORT string Description::toString() const {
    return stream.str();
}

}
}
