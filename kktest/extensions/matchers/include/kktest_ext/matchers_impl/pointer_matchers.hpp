#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_POINTER_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_POINTER_MATCHERS_HPP_

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

KKTEST_EXPORT extern detail::IsNullptrMatcher isNull;
KKTEST_EXPORT extern detail::IsNotNullptrMatcher isNotNull;

namespace detail {

class KKTEST_EXPORT IsNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj);

    void describe(Description* description);

    void describeMismatch(Description* description, void*const&);
};

class KKTEST_EXPORT IsNotNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj);

    void describe(Description* description);

    void describeMismatch(Description* description, void*const&);
};

}
}
}

#endif
