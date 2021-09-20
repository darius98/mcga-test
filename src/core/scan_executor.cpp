#include "scan_executor.hpp"

namespace mcga::test {

ScanExecutor::ScanExecutor(ExtensionApi* api): Executor(api) {
}

void ScanExecutor::execute(Test test) {
    for (const GroupPtr& group: test.getGroupStack()) {
        if (discoveredGroups.count(group->getId()) == 0) {
            continue;
        }
        discoveredGroups.insert(group->getId());
        onGroupDiscovered(group);
    }
    onTestDiscovered(test);
}

Executor::Type ScanExecutor::getType() const {
    return SCAN;
}

void ScanExecutor::finalize() {
    // Intentionally don't run destroy hooks for this executor.
}

}  // namespace mcga::test
