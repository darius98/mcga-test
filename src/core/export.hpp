#pragma once

#if defined _WIN32 || defined __CYGWIN__
#define MCGA_TEST_EXPORT __declspec(dllexport)
#define MCGA_TEST_EXPORT_WEAK __declspec(dllexport)
#else
#if __GNUC__ >= 4
#define MCGA_TEST_EXPORT __attribute__((visibility("default")))
#define MCGA_TEST_EXPORT_WEAK __attribute__((visibility("default"), weak))
#else
#define MCGA_TEST_EXPORT
#define MCGA_TEST_EXPORT_WEAK
#endif
#endif
