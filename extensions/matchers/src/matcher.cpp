#include "extensions/matchers/include/kktest_ext/matchers_impl/matcher.hpp"

using namespace std;

namespace kktest {
namespace matchers {

Description::Description() = default;

Description::Description(const Description& other):
        stream(other.stream.str()) {}

Description& Description::appendRawString(const string& str) {
    stream << str;
    return *this;
}

string Description::toString() const {
    return stream.str();
}

}
}
