#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_DEFINERS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_DEFINERS_H_

#include <string>

#include "./executable.hpp"
#include "./matcher.hpp"
#include "./config.hpp"

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

    void operator()(const TestConfig& config, Executable func);

    void operator()(std::string description, Executable func);
};

class GroupDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(const GroupConfig& config, Executable func);

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

    void operator()(const bool& result, const std::string& expr);

    template<class T, class M, class... Args, IS_MATCHER(M)>
    void operator()(const T& object, M* matcher, const Args... args) {
        checkDuringTest();
        if (matcher->matches(object)) {
            return;
        }
        Description description;
        description << file << ":" << line << ":\n\tExpected ";
        matcher->describe(description);
        description << ".\n\tGot '" << object << "'.\n\tWhich is ";
        matcher->describeMismatch(description);
        description << ".";
        throwExpectationFailed(description.toString());
    }

private:
    void throwExpectationFailed(const std::string& message);

    void checkDuringTest();
};

}

#endif
