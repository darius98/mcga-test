#include "common/interproc/src/subprocess.hpp"
#include "common/utils/src/string.hpp"

using namespace std;
using namespace kktest::utils;

namespace kktest {
namespace interproc {

Subprocess* Subprocess::open(const string& executable,
                             const vector<string>& argv) {
    char* executableCStr = copyAsCString(executable);
    auto cArgv = static_cast<char**>(malloc(sizeof(char*) * (argv.size() + 1)));
    for (size_t i = 0; i < argv.size(); ++ i) {
        cArgv[i] = copyAsCString(argv[i]);
    }
    cArgv[argv.size()] = nullptr;
    return open(executableCStr, cArgv);
}

Subprocess::FinishStatus Subprocess::getFinishStatus() {
    if (isSignaled()) {
        return SIGNALED;
    }
    if (!isExited()) {
        return UNKNOWN;
    }
    return getReturnCode() == 0 ? ZERO_EXIT : NON_ZERO_EXIT;
}

}
}
