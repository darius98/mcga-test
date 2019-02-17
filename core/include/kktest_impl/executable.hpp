#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_EXECUTABLE_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_EXECUTABLE_HPP_

#include <functional>

namespace kktest {

typedef std::function<void()> CopyableExecutable;

typedef const CopyableExecutable& Executable;

}

#endif
