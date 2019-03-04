#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_POINTER_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_POINTER_HPP_

#include <kktest_ext/matchers/matcher.hpp>
#include <kktest_ext/matchers/detail/decl.hpp>

namespace kktest {
namespace matchers {

namespace detail {

class IsNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj) {
        return obj == nullptr;
    }

    void describe(Description* description) {
        (*description) << "nullptr";
    }

    void describeMismatch(Description* description, void*const&) {
        (*description) << "non-null pointer";
    }
};

class IsNotNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj) {
        return obj != nullptr;
    }

    void describe(Description* description) {
        (*description) << "non-null pointer";
    }

    void describeMismatch(Description* description, void*const&) {
        (*description) << "nullptr";
    }
};

}

static detail::IsNullptrMatcher isNull;

static detail::IsNotNullptrMatcher isNotNull;

}
}

#endif
