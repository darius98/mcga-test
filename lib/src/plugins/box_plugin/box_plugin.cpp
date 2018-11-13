#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "box_executor.hpp"

using namespace std;

AddArgument(int, flagBoxed)
    .ArgumentType("0|1 ")
    .Name("boxed")
    .Short("b")
    .Description("Run each test in an isolated process (boxed)")
    .DefaultValue(0)
    .ImplicitValue(1);
AddArgument(int, argumentNumBoxes)
    .ArgumentType("int")
    .Name("max_parallel_tests")
    .Description("Maximum number of tests to execute in parallel "
                 "(processes to spawn) when running boxed")
    .DefaultValue("1");


namespace kktest {

class BoxPlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override {
        return flagBoxed != 0;
    }

    void install() override {
        if (argumentNumBoxes <= 0) {
            throw runtime_error("Invalid number of boxes.");
        }
        boxExecutor = new BoxExecutor((size_t)argumentNumBoxes);
        TestingDriver::addAfterInitHook([this]() {
            TestingDriver::setExecutor(boxExecutor);
        });
    }

    void uninstall() override {
        delete boxExecutor;
    }

private:
    BoxExecutor* boxExecutor = nullptr;
};

BoxPlugin boxPlugin("kktest");

}
