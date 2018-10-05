#ifndef KKTEST_TESTING_DEFINER_H_
#define KKTEST_TESTING_DEFINER_H_

#include <functional>
#include <string>

#include "matcher/matcher.hpp"
#include "testing/driver.hpp"
#include "testing/executable.hpp"


namespace kktest {

class Definer {
public:
    Definer(std::string _file, int _line);
protected:
    TestingDriver* getDriver();

    bool isDuringTest();

    std::string file;
    int line;
};

class TestDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(std::string description, Executable func);
};

class GroupDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(std::string description, Executable func);
};

class SetUpDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(Executable func);
};

class TearDownDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(Executable func);
};

class BaseExpectDefiner: public Definer {
public:
    using Definer::Definer;

protected:
    void checkDuringTest();

    void throwExpectationFailed(Description* description);
};

class ExpectDefiner: public BaseExpectDefiner {
public:
    using BaseExpectDefiner::BaseExpectDefiner;

    void operator()(const bool& result, const std::string& expr);
};

class ExpectMatchesDefiner: public BaseExpectDefiner {
public:
    using BaseExpectDefiner::BaseExpectDefiner;

    template<class T, class M, IS_MATCHER(M)>
    void operator()(const T& object, M* matcher) {
        checkDuringTest();
        if (matcher->matches(object)) {
            return;
        }
        auto description = Description::createForExpect(file, line, "");
        matcher->describe(*description);
        (*description) << ". Got '" << object << "': ";
        matcher->describeMismatch(*description);
        throwExpectationFailed(description);
    }
};

}

#endif
