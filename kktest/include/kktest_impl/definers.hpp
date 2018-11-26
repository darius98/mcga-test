#ifndef KKTEST_INCLUDE_KKTEST_IMPL_DEFINERS_H_
#define KKTEST_INCLUDE_KKTEST_IMPL_DEFINERS_H_

#include <string>

#include <kktest_impl/config.hpp>
#include <kktest_impl/executable.hpp>

namespace kktest {

class TestCaseDefiner {
public:
    explicit TestCaseDefiner(void (*testCase)());
};

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

    template<class T, class M, class E, class... Args>
    void operator()(const T& object, M matcher, const E&, const Args...) {
        if (matcher.matches(object)) {
            return;
        }
        throwExpectationFailed(file + ":" + std::to_string(line) + ":\n"
                               "\t" + matcher.buildMismatchMessage(object));
    }

private:
    void throwExpectationFailed(const std::string& message);
};

}

#endif
