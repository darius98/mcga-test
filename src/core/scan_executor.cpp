#include "scan_executor.hpp"

namespace mcga::test {

ScanExecutor::ScanExecutor(ExtensionApi* api): Executor(api, SCAN) {
}

void ScanExecutor::execute(Test test) {
    for (const GroupPtr& group: test.getGroupStack()) {
        if (discoveredGroups.count(group->getId()) == 0) {
            continue;
        }
        discoveredGroups.insert(group->getId());
        api->runHooks<ExtensionApi::ON_GROUP_DISCOVERED>(group);
    }
    api->runHooks<ExtensionApi::ON_TEST_DISCOVERED>(test);
}

}  // namespace mcga::test
