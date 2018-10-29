#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "box_module.hpp"
#include "box_executor.hpp"

AddArgument(int, flagBoxed)
    .ArgumentType("0|1 ")
    .Name("boxed")
    .Short("b")
    .Description("Run the tests boxed (requires sudo + box installed)")
    .DefaultValue(0)
    .ImplicitValue(1);
AddArgument(int, argumentFirstBox)
    .ArgumentType("int")
    .Name("first_box")
    .Description("The index of the first box to use while running boxed")
    .DefaultValue("0");
AddArgument(int, argumentNumBoxes)
    .ArgumentType("int")
    .Name("num_boxes")
    .Description("Number of boxes to use while running boxed "
                 "(consecutive ids, starting from first_box)")
    .DefaultValue("10");


namespace kktest {

bool BoxModule::isEnabled() const {
    return flagBoxed != 0;
}

void BoxModule::install() {
    TestingDriver::addAfterInitHook([]() {
        TestingDriver::setExecutor(
                new BoxExecutor(TestingDriver::getBinaryPath(),
                                argumentFirstBox,
                                argumentFirstBox + argumentNumBoxes)
        );
    });
}

}