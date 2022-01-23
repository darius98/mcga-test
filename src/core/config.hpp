#pragma once

#ifndef MCGA_TEST_PRE_ALLOCATED_GROUPS
#define MCGA_TEST_PRE_ALLOCATED_GROUPS 8
#endif

#ifndef MCGA_TEST_PRE_ALLOCATED_CALLBACKS
#define MCGA_TEST_PRE_ALLOCATED_CALLBACKS 128
#endif

namespace mcga::test {

inline constexpr auto numStaticGroups = MCGA_TEST_PRE_ALLOCATED_GROUPS;
inline constexpr auto numStaticCallbacks = MCGA_TEST_PRE_ALLOCATED_CALLBACKS;

}  // namespace mcga::test
