#include "scan_executor.hpp"

namespace mcga::test {

ScanExecutor::ScanExecutor(HooksManager* hooks): Executor(hooks) {
}

void ScanExecutor::execute(Test test) {
    for (const GroupPtr& group: test.getGroupStack()) {
        if (discoveredGroups.count(group->getId()) == 0) {
            continue;
        }
        discoveredGroups.insert(group->getId());
        hooks->runHooks<HooksManager::ON_GROUP_DISCOVERED>(group);
    }
    hooks->runHooks<HooksManager::ON_TEST_DISCOVERED>(test);
}

void ScanExecutor::emitWarning(const std::string& message, size_t groupId) {
    hooks->runHooks<HooksManager::ON_WARNING>(Warning(message, groupId));
}

Executor::Type ScanExecutor::getType() const {
    return SCAN;
}

}  // namespace mcga::test
