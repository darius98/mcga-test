#pragma once

#include "mcga/test.hpp"
#include "mcga/matchers.hpp"

namespace mcga::test {

template<class T,
  class M,
  class = std::enable_if_t<std::is_base_of_v<matchers::Matcher, M>>>
void expect(const T& obj, M matcher) {
    if (matcher.matches(obj)) {
        return;
    }
    matchers::Description description;
    description.appendRawString("Expected ");
    matcher.describe(&description);
    description.appendRawString("\n\tGot      '");
    description << obj;
    description.appendRawString("'\n\tWhich is ");
    matcher.describeFailure(&description);
    fail("Expectation failed:\n\t" + description.toString());
}

}