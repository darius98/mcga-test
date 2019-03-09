#pragma once

#include <type_traits>

#include <sstream>
#include <string>
#include <tuple>
#include <utility>

namespace kktest::matchers::tp {

namespace detail {

template<class T>
constexpr decltype(std::begin(std::declval<const T&>())) Begin();

template<class T>
constexpr decltype(std::begin(std::declval<const T&>())) End();

template<class T>
constexpr T& Ref(T);

template<class T>
decltype(Begin<T>() != End<T>(),
         ++ Ref(Begin<T>()),
         *Begin<T>(),
         std::true_type()) IsIterableImpl(int) {
    return std::true_type();
}

template <class T>
std::false_type IsIterableImpl(...) {
    return std::false_type();
}

template<class T>
struct IsPairImpl : std::false_type {};

template<class A, class B>
struct IsPairImpl<std::pair<A, B>> : std::true_type {};

template<class T>
struct IsTupleImpl : std::false_type {};

template<class... Items>
struct IsTupleImpl<std::tuple<Items...>> : std::true_type {};

template<class T>
decltype(Ref(std::declval<std::stringstream>()) << std::declval<T>(),
         std::true_type()) IsSstreamableImpl(int) {
    return std::true_type();
}

template<class T>
std::false_type IsSstreamableImpl(...) {
    return std::false_type();
}

}

template<class T>
constexpr bool IsStringLike =
    std::is_same_v<T, std::string>
    || std::is_same_v<T, std::string_view>
    || (std::is_pointer_v<T>
        && std::is_same_v<char, std::remove_pointer_t<T>>)
    || (std::is_array_v<T>
        && std::is_same_v<char, std::remove_all_extents_t<T>>);

template<class T>
constexpr bool IsIterable = decltype(detail::IsIterableImpl<T>(0))::value;

template<class T>
constexpr bool IsPair = detail::IsPairImpl<T>::value;

template<class T>
constexpr bool IsTuple = detail::IsTupleImpl<T>::value;

template<class T>
constexpr bool IsNullptrT = std::is_same_v<T, std::nullptr_t>;

template<class T>
constexpr bool IsSstreamable = decltype(detail::IsSstreamableImpl<T>(0))::value;

template<class S, std::size_t I = 0, class... Items>
typename std::enable_if<I == sizeof...(Items), void>::type
        StreamTuple(S& stream, const std::tuple<Items...>& t) {}

template<class S, std::size_t I = 0, class... Items>
typename std::enable_if<I < sizeof...(Items), void>::type
        StreamTuple(S& stream, const std::tuple<Items...>& t) {
    if (I != 0) {
        stream << ',';
    }
    stream << std::get<I>(t);
    StreamTuple<I + 1, Items...>(stream, t);
}

}
