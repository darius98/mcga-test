#ifndef KKTEST_CORE_EXTENSION_API_IMPL_HPP_
#define KKTEST_CORE_EXTENSION_API_IMPL_HPP_

#include <extension_api/extension_api.hpp>
#include "hooks.hpp"

namespace kktest {

class ExtensionApiImpl: public ExtensionApi {
 public:
    void beforeTest(const TestHook& hook) override;
    void afterTest(const TestHook& hook) override;
    void beforeGroup(const GroupHook& hook) override;
    void afterGroup(const GroupHook& hook) override;
    void afterInit(const AfterInitHook& hook) override;
    void beforeDestroy(const BeforeDestroyHook& hook) override;
    void beforeForceDestroy(const BeforeForceDestroyHook& hook) override;

    const Hooks& getHooks() const;

 private:
    Hooks hooks;
};

}

#endif
