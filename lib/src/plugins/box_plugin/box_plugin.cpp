#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "box_plugin.hpp"
#include "box_executor.hpp"

AddArgument(int, flagBoxed)
    .ArgumentType("0|1 ")
    .Name("boxed")
    .Short("b")
    .Description("Run the tests in isolated processes (boxed)")
    .DefaultValue(0)
    .ImplicitValue(1);
AddArgument(int, argumentNumBoxes)
    .ArgumentType("int")
    .Name("num_boxes")
    .Description("Number of boxes to use while running boxed "
                 "(consecutive ids, starting from first_box)")
    .DefaultValue("10");


namespace kktest {

bool BoxPlugin::isEnabled() const {
    return flagBoxed != 0;
}

void BoxPlugin::install() {
    TestingDriver::addAfterInitHook([]() {
        TestingDriver::setExecutor(
            new BoxExecutor(TestingDriver::getBinaryPath(), argumentNumBoxes)
        );
    });
}

}
