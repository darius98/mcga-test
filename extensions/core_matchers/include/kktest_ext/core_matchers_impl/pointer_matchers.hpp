#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_POINTER_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_POINTER_MATCHERS_HPP_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {
namespace core_matchers {

class IsNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj);

    void describe(Description* description);

    void describeMismatch(Description* description, void*const&);
};

extern IsNullptrMatcher isNull;

class IsNotNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj);

    void describe(Description* description);

    void describeMismatch(Description* description, void*const&);
};

extern IsNotNullptrMatcher isNotNull;

}
}

#endif
