#pragma once

#include "mcga/matchers.hpp"
#include "mcga/test.hpp"

namespace mcga::test {

template<class T, matchers::MatcherFor<T> M>
void expect(const T& obj, M matcher, Context context = Context()) {
    if (matcher.matches(obj)) {
        return;
    }
    matchers::Description description;
    description.appendRawString("Expected ");
    matcher.describe(&description);
    description.appendRawString("\nGot      '");
    description << obj;
    description.appendRawString("'\nWhich is ");
    matcher.describeFailure(&description);
    context.verb = "Expectation failed";
    fail(description.toString(), context);
}

template<class T, matchers::EqualityMatchableFor<T> Val>
void expect(const T& obj, Val expected, Context context = Context()) {
    expect(obj, matchers::isEqualTo(expected), context);
}

}  // namespace mcga::test
