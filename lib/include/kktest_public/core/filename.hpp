#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_FILENAME_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_FILENAME_H_

// TODO(darius98): Trim the file to project-level instead.
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ?                       \
                      __builtin_strrchr(__FILE__, '/') + 1 :                   \
                      __FILE__)

#endif
