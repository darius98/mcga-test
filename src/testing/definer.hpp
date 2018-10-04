#ifndef KKTEST_TESTING_DEFINER_H_
#define KKTEST_TESTING_DEFINER_H_

#include <functional>
#include <string>


namespace kktest {
namespace __internal {

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

    void operator()(std::string description, const std::function<void()>& func);
};

class GroupDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(std::string description, const std::function<void()>& func);
};

class SetUpDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(const std::function<void()>& func);
};

class TearDownDefiner: public Definer {
public:
    using Definer::Definer;

    void operator()(const std::function<void()>& func);
};

}
}

#endif
