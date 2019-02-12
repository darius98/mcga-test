#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_TRUTH_MATCHERS_HPP_

#include <kktest_ext/core_matchers_impl/matcher.hpp>
#include <kktest_ext/core_matchers_impl/detail/decl.hpp>

namespace kktest {
namespace core_matchers {

extern detail::IsTrueMatcher isTrue;
extern detail::IsFalseMatcher isFalse;

namespace detail {

class IsTrueMatcher: public Matcher {
 public:
    bool matches(const bool& object);

    void describe(Description* description);

    void describeMismatch(Description* description, const bool&);
};

class IsFalseMatcher: public Matcher {
 public:
    bool matches(const bool& object);

    void describe(Description* description);

    void describeMismatch(Description* description, const bool&);
};

}
}
}

#endif
