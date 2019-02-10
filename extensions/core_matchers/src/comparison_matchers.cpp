#include <kktest_ext/core_matchers_impl/comparison_matchers.hpp>

using namespace std;

namespace {

constexpr size_t relevantRange = 20;

string extractRelevantSection(const string& s, const size_t index) {
    auto firstIndex = index >= relevantRange ? index - relevantRange : 0;
    auto lastIndex = min(index + relevantRange, s.size() - 1);
    auto relevantSection = s.substr(firstIndex, lastIndex - firstIndex + 1);

    if (index + relevantRange + 1 < s.size()) {
        relevantSection += "...";
    }
    if (index > relevantRange) {
        relevantSection = "..." + relevantSection;
    }
    return relevantSection;
}

}

namespace kktest {
namespace core_matchers {

EqualityMatcher<string>::EqualityMatcher(string _target):
        target(move(_target)) {}

bool EqualityMatcher<string>::matches(const string& obj) {
    object = obj;
    return object == target;
}

bool EqualityMatcher<string>::matches(const char* obj) {
    object = obj;
    return object == target;
}

void EqualityMatcher<string>::describe(Description* description) {
    (*description) << "'" << target << "'";
}

void EqualityMatcher<string>::describeMismatch(Description* description) {
    size_t mismatchIndex = 0;
    while (mismatchIndex < min(target.size(), object.size())
            && target[mismatchIndex] == object[mismatchIndex]) {
        mismatchIndex += 1;
    }

    // number of spaces we must place the mismatch caret to the right
    // the initial '11' represents the length of "Expected: '" / "     Got: '"
    size_t numSpaces = 11u + min(mismatchIndex, relevantRange);
    if (mismatchIndex > relevantRange) {
        numSpaces += 3;
    }
    // TODO(darius98): There is a problem with the design here: we need to count
    //             ... how many characters will be changed by the Streamer
    //             ... implementation to be represented here by two characters
    //             ... (so we know where to position the mismatch caret).
    //             ... This class should not be inter-dependent with the
    //             ... implementation of Streamer.
    for (size_t index = mismatchIndex >= relevantRange
                        ? mismatchIndex - relevantRange
                        : 0;
         index < mismatchIndex;
         ++ index) {
        if (target[index] == '\n'
                || target[index] == '\r'
                || target[index] == '\t') {
            numSpaces += 1;
        }
    }

    (*description) << "different at index " << mismatchIndex << ":";
    description->appendRawString("\n\tExpected: '");
    (*description) << extractRelevantSection(target, mismatchIndex);
    description->appendRawString("'\n\t     Got: '");
    (*description) << extractRelevantSection(object, mismatchIndex);
    description->appendRawString("'\n\t" + String(numSpaces, ' ') + "^");
}

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
