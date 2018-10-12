#ifndef KKTEST_TESTING_DEFINER_H_
#define KKTEST_TESTING_DEFINER_H_

#include <string>

#include "executable.hpp"


namespace kktest {

class Definer {
public:
    Definer(std::string _file, int _line);
protected:
    /**
     * This method returns TestingDriver::getGlobalDriver().
     *
     * It returns it as a void pointer as a trick to avoid including driver.hpp
     * in this file (that is exposed in the API of the library), to
     * significantly reduce compile time on user-implemented test suites.
     */
    void* getDriverAsVoid() const;

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
