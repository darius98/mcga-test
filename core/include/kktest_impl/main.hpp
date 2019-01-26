#ifndef KKTEST_CORE_KKTEST_IMPL_MAIN_HPP_
#define KKTEST_CORE_KKTEST_IMPL_MAIN_HPP_

#include <vector>

#include <kktest_impl/extension_api.hpp>

namespace kktest {

struct InternalArgs {
    cppli::Flag* versionFlag = nullptr;
    cppli::Flag* getSignatureFlag = nullptr;
    cppli::Flag* boxedFlag = nullptr;
    cppli::NumericArgument<int>* maxParallelTestsArgument = nullptr;
};

InternalArgs registerInternalFlags(cppli::Cppli* cliApi);

int main(const std::vector<Extension*>& extensions, InternalArgs internalFlags);

}

#endif
