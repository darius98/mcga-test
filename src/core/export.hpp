#pragma once

#if defined _WIN32 || defined __CYGWIN__
#define MCGA_TEST_EXPORT __declspec(dllexport)
#else
#if __GNUC__ >= 4
#define MCGA_TEST_EXPORT __attribute__((visibility("default")))
#else
#define MCGA_TEST_EXPORT
#endif
#endif
