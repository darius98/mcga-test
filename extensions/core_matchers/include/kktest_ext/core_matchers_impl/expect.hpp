#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_EXPECT_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_EXPECT_HPP_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {
namespace core_matchers {

template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<core_matchers::Matcher, M>::value
                >
        >
void expect(const T& object, M matcher) {
    if (matcher.matches(object)) {
        return;
    }
    core_matchers::Description description;
    description << "Expected ";
    matcher.describe(&description);
    description << "\n\tGot      '";
    core_matchers::detail::__describeObject(matcher, &description, object);
    description << "'\n";
    core_matchers::detail::__describeMismatch(matcher, &description, object);
    fail("Expectation failed:\n\t" + description.toString());
}

}
}

#endif
