#ifndef RUNTIME_TESTING_TESTING_DEFINER_H_
#define RUNTIME_TESTING_TESTING_DEFINER_H_

#include <string>
#include <functional>


namespace runtime_testing {
namespace __internal {

class TestDefiner {
public:
    explicit TestDefiner(std::string _file="", int _line=0);

    void operator()(std::string description, const std::function<void()>& func);
private:
    std::string file;
    int line;
};

class GroupDefiner {
public:
    explicit GroupDefiner(std::string _file="", int _line=0);

    void operator()(std::string description, const std::function<void()>& func);
private:
    std::string file;
    int line;
};

}
}

#endif
