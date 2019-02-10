#include <kktest_ext/core_matchers_impl/matcher.hpp>

using namespace std;

namespace kktest {
namespace core_matchers {

Description::Description() = default;

Description::Description(const Description& other):
        stream(other.stream.str()) {}

Description& Description::appendRawString(const String& str) {
    stream << str;
    return *this;
}

String Description::toString() const {
    return String(stream.str());
}

}
}
