#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "box_plugin.hpp"

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

Plugin* boxPlugin = new BoxPlugin("kktest");

bool BoxPlugin::isEnabled() const {
    return flagBoxed != 0;
}

void BoxPlugin::install() {
    if (argumentNumBoxes <= 0) {
        throw runtime_error("Invalid number of boxes.");
    }
    boxExecutor = new BoxExecutor((size_t)argumentNumBoxes);
    TestingDriver::addAfterInitHook([this]() {
        TestingDriver::setExecutor(boxExecutor);
    });
}

void BoxPlugin::uninstall() {
    delete boxExecutor;
}

}
