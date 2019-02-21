#ifndef KKTEST_CORE_INCLUDE_KKTEST_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_HPP_

#include <functional>
#include <string>
#include <vector>

#define KKTEST_VERSION "1.0.0"

#if defined _WIN32 || defined __CYGWIN__
    #define KKTEST_EXPORT __declspec(dllexport)
#else
    #if __GNUC__ >= 4
        #define KKTEST_EXPORT __attribute__((visibility("default")))
    #else
        #define KKTEST_EXPORT
    #endif
#endif

namespace kktest {

typedef const std::function<void()>& Executable;

struct KKTEST_EXPORT TestConfig {
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;

    TestConfig() = default;
    TestConfig(std::string description); // NOLINT(google-explicit-constructor)
    TestConfig(const char* description); // NOLINT(google-explicit-constructor)

    TestConfig& setDescription(std::string description);
    TestConfig& setOptional(bool optional);
    TestConfig& setTimeTicksLimit(double timeTicksLimit);
};

struct KKTEST_EXPORT GroupConfig {
    std::string description = "-";

    GroupConfig() = default;
    GroupConfig(std::string description); // NOLINT(google-explicit-constructor)
    GroupConfig(const char* description); // NOLINT(google-explicit-constructor)

    GroupConfig& setDescription(std::string description);
};

struct KKTEST_EXPORT TestCase {
    std::function<void()> exec;
    std::string name;

    TestCase(std::function<void()> exec); // NOLINT(google-explicit-constructor)
    TestCase(void (*exec)()); // NOLINT(google-explicit-constructor)
    TestCase(std::function<void()> exec, std::string name);

    virtual void run();
};

KKTEST_EXPORT void test(TestConfig config, Executable func);

KKTEST_EXPORT void group(GroupConfig config, Executable func);

KKTEST_EXPORT void setUp(Executable func);

KKTEST_EXPORT void tearDown(Executable func);

KKTEST_EXPORT void fail(const std::string& message=std::string());

KKTEST_EXPORT void expect(bool expr, const std::string& message=std::string());

KKTEST_EXPORT int run(int argc, char **argv, std::vector<TestCase>&& tests);

}

#endif
