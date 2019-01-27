#ifndef KKTEST_CORE_KKTEST_IMPL_MAIN_HPP_
#define KKTEST_CORE_KKTEST_IMPL_MAIN_HPP_

#include <vector>

#include <kktest_impl/extension_api.hpp>

namespace kktest {

struct InternalArgs {
    cppli::Flag boxedFlag;
    cppli::NumericArgument<int> maxParallelTestsArgument;
};

InternalArgs registerInternalFlags(cppli::Cppli& cppli,
                                   const std::string& versionString);

int main(const std::vector<Extension*>& extensions, InternalArgs internalFlags);

}

#endif
