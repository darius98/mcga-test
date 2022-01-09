#include "mcga/test.hpp"

#include <csignal>

auto testCase = mcga::test::TestCase{"TestCase"} +
  [] { mcga::test::test("test", [] { raise(SIGTERM); }); };
