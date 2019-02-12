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
namespace detail {

EqualityMatcher<string>::EqualityMatcher(string _target):
        target(move(_target)) {}

bool EqualityMatcher<string>::matches(const string& obj) {
    return obj == target;
}

bool EqualityMatcher<string>::matches(const char* obj) {
    return obj == target;
}

void EqualityMatcher<string>::describe(Description* description) {
    (*description) << "'" << target << "'";
}

void EqualityMatcher<string>::describeMismatch(Description* description,
                                               const string& obj) {
    size_t mismatchIndex = 0;
    while (mismatchIndex < min(target.size(), obj.size())
            && target[mismatchIndex] == obj[mismatchIndex]) {
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
    (*description) << extractRelevantSection(obj, mismatchIndex);
    description->appendRawString("'\n\t" + String(numSpaces, ' ') + "^");
}

NonEqualityMatcher<string>::NonEqualityMatcher(string _target):
        target(move(_target)) {}

bool NonEqualityMatcher<string>::matches(const string& obj) {
    return obj != target;
}

bool NonEqualityMatcher<string>::matches(const char* obj) {
    return string(obj) != target;
}

void NonEqualityMatcher<string>::describe(Description* description) {
    (*description) << "not '" << target << "'";
}

void NonEqualityMatcher<string>::describeMismatch(Description* description,
                                                  const string&) {
    (*description) << "'" << target << "'";
}

IsLessThanMatcher<string>::IsLessThanMatcher(string _target):
        target(move(_target)) {}

bool IsLessThanMatcher<string>::matches(const string& obj) {
    return obj < target;
}

bool IsLessThanMatcher<string>::matches(const char* obj) {
    return string(obj) < target;
}

void IsLessThanMatcher<string>::describe(Description* description) {
    (*description) << "< '" << target << "'";
}

void IsLessThanMatcher<string>::describeMismatch(Description* description,
                                                 const string&) {
    (*description) << ">= '" << target << "'";
}

IsLessThanEqualMatcher<string>::IsLessThanEqualMatcher(string _target):
        target(move(_target)) {}

bool IsLessThanEqualMatcher<string>::matches(const string& obj) {
    return obj <= target;
}

bool IsLessThanEqualMatcher<string>::matches(const char* obj) {
    return string(obj) <= target;
}

void IsLessThanEqualMatcher<string>::describe(Description* description) {
    (*description) << "<= '" << target << "'";
}

void IsLessThanEqualMatcher<string>::describeMismatch(Description* description,
                                                      const string&) {
    (*description) << "> '" << target << "'";
}

IsGreaterThanMatcher<string>::IsGreaterThanMatcher(string _target):
        target(move(_target)) {}

bool IsGreaterThanMatcher<string>::matches(const string& obj) {
    return obj > target;
}

bool IsGreaterThanMatcher<string>::matches(const char* obj) {
    return string(obj) > target;
}

void IsGreaterThanMatcher<string>::describe(Description* description) {
    (*description) << "> '" << target << "'";
}

void IsGreaterThanMatcher<string>::describeMismatch(Description* description,
                                                    const string& obj) {
    (*description) << "<= '" << target << "'";
}

IsGreaterThanEqualMatcher<string>::IsGreaterThanEqualMatcher(string _target):
        target(move(_target)) {}

bool IsGreaterThanEqualMatcher<string>::matches(const string& obj) {
    return obj >= target;
}

bool IsGreaterThanEqualMatcher<string>::matches(const char* obj) {
    return string(obj) >= target;
}

void IsGreaterThanEqualMatcher<string>::describe(Description* description) {
    (*description) << ">= '" << target << "'";
}

void IsGreaterThanEqualMatcher<string>::describeMismatch(
        Description* description,
        const string&) {
    (*description) << "< '" << target << "'";
}

}
}
}
