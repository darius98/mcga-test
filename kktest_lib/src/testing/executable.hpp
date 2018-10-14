#ifndef KKTEST_TESTING_EXECUTABLE_H_
#define KKTEST_TESTING_EXECUTABLE_H_

#include <functional>

namespace kktest {

typedef std::function<void()> CopyableExecutable;

typedef const CopyableExecutable& Executable;

}

#endif
