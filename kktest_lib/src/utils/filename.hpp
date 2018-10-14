#ifndef KKTEST_UTILS_FILENAME_H_
#define KKTEST_UTILS_FILENAME_H_

// TODO(darius98): Trim the file to project-level instead.
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ?                       \
                      __builtin_strrchr(__FILE__, '/') + 1 :                   \
                      __FILE__)

#endif
