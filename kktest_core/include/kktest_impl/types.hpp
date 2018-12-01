#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_TYPES_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_TYPES_H_

#include <functional>
#include <string>

namespace kktest {

typedef std::string String;

typedef std::function<void()> CopyableExecutable;

typedef const CopyableExecutable& Executable;

}

#endif
