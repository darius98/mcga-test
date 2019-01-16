#include <kktest_ext/core_matchers_impl/comparison_matchers.hpp>

using namespace std;

namespace kktest {
namespace core_matchers {

EqualityMatcher<string> isEqualTo(const char obj[]) {
    return EqualityMatcher<string>(string(obj));
}

NonEqualityMatcher<string> isNotEqualTo(const char obj[]) {
    return NonEqualityMatcher<string>(string(obj));
}

IsLessThanMatcher<string> isLessThan(const char obj[]) {
    return IsLessThanMatcher<string>(string(obj));
}

IsLessThanEqualMatcher<string> isLessThanEqual(const char obj[]) {
    return IsLessThanEqualMatcher<string>(string(obj));
}

IsGreaterThanMatcher<string> isGreaterThan(const char obj[]) {
    return IsGreaterThanMatcher<string>(string(obj));
}

IsGreaterThanEqualMatcher<string> isGreaterThanEqual(const char obj[]) {
    return IsGreaterThanEqualMatcher<string>(string(obj));
}

}
}
