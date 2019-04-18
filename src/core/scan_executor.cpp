#include "scan_executor.hpp"

namespace mcga::test {

void ScanExecutor::execute(Test test) {
    // std::cout << "Scanned test " << test.getId() << "\n";
}

Executor::Type ScanExecutor::getType() const {
    return SCAN;
}

} // namespace mcga::test
