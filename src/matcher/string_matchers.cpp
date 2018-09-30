#include "string_matchers.hpp"

#include <cstring>

using namespace std;

namespace matcher {

CharInStringMatcher::CharInStringMatcher(const char* _container,
                                         const char* _expectation) noexcept:
        container(_container), expectation(_expectation) {}

bool CharInStringMatcher::matches(const char& ch) {
    return strchr(container, ch) != nullptr;
}

void CharInStringMatcher::describe(Description& description) {
    if (expectation != nullptr) {
        description << expectation;
    } else {
        description << "character in '" << container << "'";
    }
}

void CharInStringMatcher::describeMismatch(Description& description) {
    if (expectation != nullptr) {
        description << "not " << expectation;
    } else {
        description << "character that is not in '" << container << "'";
    }
}

IsSubstringMatcher::IsSubstringMatcher(string _container):
    container(move(_container)) {}

bool IsSubstringMatcher::matches(const string& object) {
    return container.find(object) != string::npos;
}

void IsSubstringMatcher::describe(Description& description) {
    description << "a substring of '" << container << "'";
}

void IsSubstringMatcher::describeMismatch(Description& description) {
    description << "not a substring of '" << container << "'";
};

}
