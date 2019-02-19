#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_EXPORT_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_EXPORT_HPP_

#if defined _WIN32 || defined __CYGWIN__
    #define KKTEST_EXPORT __declspec(dllexport)
#else
    #if __GNUC__ >= 4
        #define KKTEST_EXPORT __attribute__((visibility("default")))
    #else
        #define KKTEST_EXPORT
    #endif
#endif

#endif
