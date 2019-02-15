#ifndef KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_POINTER_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_POINTER_MATCHERS_HPP_

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

extern KKTEST_API detail::IsNullptrMatcher isNull;
extern KKTEST_API detail::IsNotNullptrMatcher isNotNull;

namespace detail {

class KKTEST_API IsNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj);

    void describe(Description* description);

    void describeMismatch(Description* description, void*const&);
};

class KKTEST_API IsNotNullptrMatcher: public Matcher {
 public:
    bool matches(void*const& obj);

    void describe(Description* description);

    void describeMismatch(Description* description, void*const&);
};

}
}
}

#endif
