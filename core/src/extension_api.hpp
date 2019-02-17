#ifndef KKTEST_CORE_SRC_EXTENSION_API_HPP_
#define KKTEST_CORE_SRC_EXTENSION_API_HPP_

#include <functional>
#include <stdexcept>

#include <cppli.hpp>

#include "core/src/info.hpp"

namespace kktest {

class ExtensionApi {
 public:
    typedef std::function<void(const TestInfo&)> TestHook;
    typedef std::function<void(const GroupInfo&)> GroupHook;
    typedef std::function<void()> AfterInitHook;
    typedef std::function<void()> BeforeDestroyHook;
    typedef std::function<void(const std::exception&)> BeforeForceDestroyHook;

    virtual ~ExtensionApi() = default;

    virtual void beforeTest(const TestHook& hook) = 0;
    virtual void afterTest(const TestHook& hook) = 0;
    virtual void beforeGroup(const GroupHook& hook) = 0;
    virtual void afterGroup(const GroupHook& hook) = 0;
    virtual void afterInit(const AfterInitHook& hook) = 0;
    virtual void beforeDestroy(const BeforeDestroyHook& hook) = 0;
    virtual void beforeForceDestroy(const BeforeForceDestroyHook& hook) = 0;
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
