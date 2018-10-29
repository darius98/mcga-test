#ifndef KKTEST_MODULES_MATCHER_MODULE_MATCHER_MODULE_H_
#define KKTEST_MODULES_MATCHER_MODULE_MATCHER_MODULE_H_

#include <core/module.hpp>

namespace kktest {

class MatcherModule: public Module {
public:
    void install() override;
};

}

#endif
