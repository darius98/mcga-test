#ifndef KKTEST_CORE_SRC_EXTENSION_API_HPP_
#define KKTEST_CORE_SRC_EXTENSION_API_HPP_

#include <functional>
#include <stdexcept>

#include <cppli.hpp>

#include "core/src/hooks.hpp"
#include "core/src/info.hpp"

namespace kktest {

class ExtensionApi {
 public:
    void beforeTest(const Hooks::TestHook& hook);
    void afterTest(const Hooks::TestHook& hook);
    void beforeGroup(const Hooks::GroupHook& hook);
    void afterGroup(const Hooks::GroupHook& hook);
    void afterInit(const Hooks::AfterInitHook& hook);
    void beforeDestroy(const Hooks::BeforeDestroyHook& hook);
    void beforeForceDestroy(const Hooks::BeforeForceDestroyHook& hook);

    const Hooks& getHooks() const;

 private:
    Hooks hooks;
};

class Extension {
 public:
    virtual ~Extension();

    virtual void registerCommandLineArgs(cppli::Parser& parser);

    virtual void init(ExtensionApi* api);

    virtual void destroy();
};

}

#endif
