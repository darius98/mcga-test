#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_DETAIL_STD_INVOKE_POLYFILL_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_DETAIL_STD_INVOKE_POLYFILL_HPP_

#include <utility>
#include <type_traits>

namespace kktest {
namespace core_matchers {
namespace detail {

template <class T>
struct isReferenceWrapper : std::false_type {};

template <class U>
struct isReferenceWrapper<std::reference_wrapper<U>> : std::true_type {};

template<class Base, class Derived>
struct isDecayDerived: public std::is_base_of<
        Base,
        typename std::decay<Derived>::type> {};

template<class Base, class Derived>
struct enableIfDecayDerived: public std::enable_if<
        isDecayDerived<Base, Derived>::value> {};

template<class Base, class Derived>
struct enableIfNotDecayDerived: public std::enable_if<
        !isDecayDerived<Base, Derived>::value> {};

template<
    class T,
    class Type,
    class T1,
    class... Args,
    class=typename std::enable_if<
            std::is_member_function_pointer<Type T::*>::value
          >::type,
    class=typename enableIfDecayDerived<T, T1>::type>
auto invokePolyfillImpl(Type T::* f, T1&& t1, Args&&... args)
        -> decltype((std::forward<T1>(t1).*f)(std::forward<Args>(args)...)) {
    return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
}

template<
    class T,
    class Type,
    class T1,
    class... Args,
    class=typename std::enable_if<
            std::is_member_function_pointer<Type T::*>::value
          >::type,
    class=typename enableIfNotDecayDerived<T, T1>::type,
    class=typename std::enable_if<
            isReferenceWrapper<typename std::decay<T1>::type>::value
          >::type>
auto invokePolyfillImpl(Type T::* f, T1&& t1, Args&&... args)
        -> decltype((t1.get().*f)(std::forward<Args>(args)...)) {
    return (t1.get().*f)(std::forward<Args>(args)...);
}

template<
    class T,
    class Type,
    class T1,
    class... Args,
    class=typename std::enable_if<
            std::is_member_function_pointer<Type T::*>::value
          >::type,
    class=typename enableIfNotDecayDerived<T, T1>::type,
    class=typename std::enable_if<
            !isReferenceWrapper<typename std::decay<T1>::type>::value
          >::type>
auto invokePolyfillImpl(Type T::* f, T1&& t1, Args&&... args)
        -> decltype(((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...)) {
    return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
}

template<
    class T,
    class Type,
    class T1,
    class... Args,
    class=typename std::enable_if<
            !std::is_member_function_pointer<Type T::*>::value
          >::type,
    class=typename enableIfDecayDerived<T, T1>::type>
auto invokePolyfillImpl(Type T::* f, T1&& t1, Args&&... args)
        -> decltype(std::forward<T1>(t1).*f) {
    static_assert(std::is_member_object_pointer<decltype(f)>::value,
                  "Must be member object pointer.");
    static_assert(sizeof...(args) == 0, "Cannot call with arguments.");
    return std::forward<T1>(t1).*f;
}

template<
    class T,
    class Type,
    class T1,
    class... Args,
    class=typename std::enable_if<
            !std::is_member_function_pointer<Type T::*>::value
          >::type,
    class=typename enableIfNotDecayDerived<T, T1>::type,
    class=typename std::enable_if<
            isReferenceWrapper<typename std::decay<T1>::type>::value
          >::type>
auto invokePolyfillImpl(Type T::* f, T1&& t1, Args&&... args)
        -> decltype(t1.get().*f) {
    static_assert(std::is_member_object_pointer<decltype(f)>::value,
                  "Must be member object pointer.");
    static_assert(sizeof...(args) == 0, "Cannot call with arguments.");
    return t1.get().*f;
}

template<
    class T,
    class Type,
    class T1,
    class... Args,
    class=typename std::enable_if<
            !std::is_member_function_pointer<Type T::*>::value
          >::type,
    class=typename enableIfNotDecayDerived<T, T1>::type,
    class=typename std::enable_if<
            !isReferenceWrapper<typename std::decay<T1>::type>::value
          >::type>
auto invokePolyfillImpl(Type T::* f, T1&& t1, Args&&... args)
        -> decltype((*std::forward<T1>(t1)).*f) {
    static_assert(std::is_member_object_pointer<decltype(f)>::value,
                  "Must be member object pointer.");
    static_assert(sizeof...(args) == 0, "Cannot call with arguments.");
    return (*std::forward<T1>(t1)).*f;
}

template <class F, class... Args>
auto invokePolyfillImpl(F&& f, Args&&... args)
        -> decltype(std::forward<F>(f)(std::forward<Args>(args)...)) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

template< class F, class... Args>
auto invokePolyfill(F&& f, Args&&... args)
        -> decltype(invokePolyfillImpl(std::forward<F>(f),
                                       std::forward<Args>(args)...)) {
    return invokePolyfillImpl(std::forward<F>(f), std::forward<Args>(args)...);
}

}
}
}

#endif
