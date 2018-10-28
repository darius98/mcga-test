#ifndef KKTEST_CORE_DEFINER_H_
#define KKTEST_CORE_DEFINER_H_

#include <string>

#include "executable.hpp"


namespace kktest {

class Definer {
public:
    Definer(std::string _file, int _line);

protected:
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

class ExpectDefiner: public Definer {
public:
    using Definer::Definer;

    virtual void operator()(const bool& result, const std::string& expr);
};

}

#endif
