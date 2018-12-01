#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_MAIN_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_MAIN_H_

#include <vector>

#include <kktest_impl/extension_api.hpp>

namespace kktest {

struct InternalArgs {
    Flag* versionFlag = nullptr;
    Flag* getSignatureFlag = nullptr;
    Flag* boxedFlag = nullptr;
    Argument* maxParallelTestsArgument = nullptr;
};

InternalArgs registerInternalFlags(ArgumentsApi* argumentsApi);

int main(const std::vector<Extension*>& extensions, InternalArgs internalFlags);

}

#endif
