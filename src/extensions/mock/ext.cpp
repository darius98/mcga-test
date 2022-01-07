#include "ext.hpp"

namespace mcga::test::mock {

namespace internal {

// Not exposed in any header.
void run_after_test_cleanups();

}  // namespace internal

void MockExtension::init(ExtensionApi* api) {
    api->addHook<ExtensionApi::AFTER_TEST_TEARDOWN>(
      [](const Test&) { internal::run_after_test_cleanups(); });
}

}  // namespace mcga::test::mock
