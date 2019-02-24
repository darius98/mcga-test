#ifndef KKTEST_CORE_SRC_EXTENSION_API_HPP_
#define KKTEST_CORE_SRC_EXTENSION_API_HPP_

#include <functional>
#include <stdexcept>
#include <vector>

#include <cppli.hpp>

#include "core/src/test.hpp"

namespace kktest {

class ExtensionApi {
 public:
    typedef std::function<void(const Test&)> BeforeTest;
    typedef std::function<void(const ExecutedTest&)> AfterTest;
    typedef std::function<void(GroupPtr)> BeforeGroup;
    typedef std::function<void(GroupPtr)> AfterGroup;
    typedef std::function<void()> AfterInit;
    typedef std::function<void()> BeforeDestroy;
    typedef std::function<void(const std::exception&)> BeforeForceDestroy;

    enum Type {
        AFTER_INIT = 0,
        BEFORE_GROUP = 1,
        AFTER_GROUP = 2,
        BEFORE_TEST = 3,
        AFTER_TEST = 4,
        BEFORE_DESTROY = 5,
        BEFORE_FORCE_DESTROY = 6,
    };

    template<Type t, class H>
    void addHook(const H& hook) {
        std::get<t>(hooks).push_back(hook);
    }

    template<Type t, class... Args>
    void runHooks(const Args&... args) {
        for (const auto& hook : std::get<t>(hooks)) {
            hook(args...);
        }
    }

 private:
    std::tuple<
        std::vector<AfterInit>,
        std::vector<BeforeGroup>,
        std::vector<AfterGroup>,
        std::vector<BeforeTest>,
        std::vector<AfterTest>,
        std::vector<BeforeDestroy>,
        std::vector<BeforeForceDestroy>
    > hooks;
};

class Extension {
 public:
    virtual ~Extension() = default;

    virtual void registerCommandLineArgs(cppli::Parser&) {}

    virtual void init(ExtensionApi&) {}

    virtual void destroy() {}
};

}

#endif
