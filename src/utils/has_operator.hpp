#ifndef UTILS_HAS_OPERATOR_H_
#define UTILS_HAS_OPERATOR_H_

#include <utility>

namespace testing {
namespace utils {

// This code is taken (and adapted) from an answer to this question:
// https://stackoverflow.com/questions/6534041

namespace __internal {

template<class LHS, class RHS, class OP>
struct IsOpValidImpl {
    template<class U, class L, class R>
    static auto test(int)
    -> decltype(
    std::declval<U>()(std::declval<L>(), std::declval<R>()),
            void(),
            std::true_type()
    ) {}

    template<class U, class L, class R>
    static auto test(...)
    -> std::false_type {}

    using type = decltype(test<OP, LHS, RHS>(0));
};

template<class LHS, class RHS, class OP>
using IsOpValid = typename IsOpValidImpl<LHS, RHS, OP>::type;

struct LSH {
    template<class LHS, class RHS>
    constexpr auto operator()(LHS &&l, RHS &&r) const
    noexcept(noexcept(std::forward<LHS>(l) << std::forward<RHS>(r)))
    -> decltype(std::forward<LHS>(l) << std::forward<RHS>(r)) {
        return std::forward<LHS>(l) << std::forward<RHS>(r);
    }
};

} // namespace __internal

template<class LHS, class RHS>
using hasLeftShift = __internal::IsOpValid<LHS, RHS, __internal::LSH>;

} // namespace utils
} // namespace testing

#endif
