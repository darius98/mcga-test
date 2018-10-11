#ifndef KKTEST_TESTING_DEFINER_H_
#define KKTEST_TESTING_DEFINER_H_

#include <functional>
#include <string>

#include "driver.hpp"
#include "executable.hpp"


namespace kktest {

class Definer {
public:
    Definer(std::string _file, int _line);
protected:
    TestingDriver* getDriver() const;

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
};

class ExpectDefiner: public BaseExpectDefiner {
public:
    using BaseExpectDefiner::BaseExpectDefiner;

    void operator()(const bool& result, const std::string& expr);
};

}

#endif
