#include "scan_executor.hpp"

namespace mcga::test {

ScanExecutor::ScanExecutor(): Executor(SCAN) {
}

void ScanExecutor::discoverGroup(GroupPtr group) {
    if (group == nullptr) {
        return;
    }
    if (group->hasParentGroup()) {
        discoverGroup(group->getParentGroup());
    }
    if (!discoveredGroups.count(group->getId())) {
        discoveredGroups.insert(group->getId());
        api->runHooks<ExtensionApi::ON_GROUP_DISCOVERED>(group);
    }
}

void ScanExecutor::execute(Test test) {
    discoverGroup(test.getGroup());
    api->runHooks<ExtensionApi::ON_TEST_DISCOVERED>(test);
}

}  // namespace mcga::test
