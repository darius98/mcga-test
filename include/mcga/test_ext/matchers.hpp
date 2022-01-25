#pragma once

#include "mcga/matchers.hpp"
#include "mcga/test.hpp"

namespace mcga::test {

namespace internal {

template<class Fail, class T, matchers::MatcherFor<T> M>
void expectMatches(Fail f,
                   const T& obj,
                   M matcher,
                   Context context = Context("Expectation failed")) {
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
    f(description.toString(), context);
}

}  // namespace internal

template<class T, matchers::MatcherFor<T> M>
void expect(const T& obj,
            M matcher,
            Context context = Context("Expectation failed")) {
    internal::expectMatches(fail, obj, matcher, context);
}

template<class T, matchers::EqualityMatchableFor<T> Val>
void expect(const T& obj,
            Val expected,
            Context context = Context("Expectation failed")) {
    internal::expectMatches(fail, obj, matchers::isEqualTo(expected), context);
}

}  // namespace mcga::test
