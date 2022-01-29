#pragma once

#ifndef MCGA_TEST_PRE_ALLOCATED_GROUPS
#define MCGA_TEST_PRE_ALLOCATED_GROUPS 8
#endif

#ifndef MCGA_TEST_PRE_ALLOCATED_CALLBACKS
#define MCGA_TEST_PRE_ALLOCATED_CALLBACKS 128
#endif

#ifndef MCGA_TEST_PRE_ALLOCATED_TEST_CASES
#define MCGA_TEST_PRE_ALLOCATED_TEST_CASES 32
#endif

#ifndef MCGA_TEST_EXCEPTIONS
#if defined(__EXCEPTIONS)
#define MCGA_TEST_EXCEPTIONS 1
#else
#define MCGA_TEST_EXCEPTIONS 0
#endif
#endif

#ifndef MCGA_TEST_THREADING
#define MCGA_TEST_THREADING __STDC_HOSTED__
#endif

#ifndef MCGA_TEST_TIMING
#define MCGA_TEST_TIMING __STDC_HOSTED__
#endif

namespace mcga::test {

inline constexpr auto numStaticGroups = MCGA_TEST_PRE_ALLOCATED_GROUPS;
inline constexpr auto numStaticCallbacks = MCGA_TEST_PRE_ALLOCATED_CALLBACKS;
inline constexpr auto numStaticWarningNotes
  = MCGA_TEST_PRE_ALLOCATED_GROUPS + 1;
inline constexpr auto numStaticTestCases = MCGA_TEST_PRE_ALLOCATED_TEST_CASES;

}  // namespace mcga::test
