#ifndef KKTEST_INCLUDE_KKTEST_EXECUTABLE_H_
#define KKTEST_INCLUDE_KKTEST_EXECUTABLE_H_

#include <functional>

namespace kktest {

typedef std::function<void()> CopyableExecutable;

typedef const CopyableExecutable& Executable;

}

#endif
