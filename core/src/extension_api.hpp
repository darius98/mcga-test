#ifndef KKTEST_CORE_SRC_EXTENSION_API_HPP_
#define KKTEST_CORE_SRC_EXTENSION_API_HPP_

#include <functional>
#include <stdexcept>

#include <cppli.hpp>

#include "core/src/hooks.hpp"

namespace kktest {

class ExtensionApi {
 public:
    void beforeTest(const Hooks::BeforeTest& hook);
    void afterTest(const Hooks::AfterTest& hook);
    void beforeGroup(const Hooks::BeforeGroup& hook);
    void afterGroup(const Hooks::AfterGroup& hook);
    void afterInit(const Hooks::AfterInit& hook);
    void beforeDestroy(const Hooks::BeforeDestroy& hook);
    void beforeForceDestroy(const Hooks::BeforeForceDestroy& hook);

    const Hooks& getHooks() const;

 private:
    Hooks hooks;
};

class Extension {
 public:
    virtual ~Extension() = default;

    virtual void registerCommandLineArgs(cppli::Parser& parser);

    virtual void init(ExtensionApi* api);

    virtual void destroy();
};

}

#endif
