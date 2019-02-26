#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include "kktest/core/include/kktest.hpp"
#include "kktest/core/src/group.hpp"

namespace kktest {

/** Immutable class holding the internal representation of a test.
 *
 * This class should only be instantiated once for every call to test() from
 * the API of the library, as an upgrade to the TestConfig instance passed to
 * the test() function. This class' copy constructor and assignment operator are
 * explicitly deleted, as it should never need to be copied throughout the
 * lifecycle of a test. After execution, this is again upgraded to an
 * ExecutedTest.
 *
 * This class privately inherits TestConfig because TestConfig exposes public
 * mutability(TestConfig#setDescription(), TestConfig#setOptional() etc.), and
 * the desired behaviour for a Test is to be immutable once instantiated. */
class Test : private TestConfig {
 public:
    /** Default constructor from a TestConfig and the metadata received from the
     * testing Driver. */
    Test(TestConfig&& config, Executable body, GroupPtr group, int index);

    Test(Test&&) = default;
    Test& operator=(Test&&) = default;

    Test(const Test&) = delete;
    Test& operator=(const Test&) = delete;

    /** See TestConfig#description. */
    const std::string& getDescription() const;

    /** See TestConfig#optional. */
    bool isOptional() const;

    /** See TestConfig#timeTicksLimit. */
    double getTimeTicksLimit() const;

    /** See TestConfig#attempts. */
    std::size_t getNumAttempts() const;

    /** See TestConfig#passedAttemptsRequired */
    std::size_t getNumRequiredPassedAttempts() const;

    /** The group that contains this test. */
    GroupPtr getGroup() const;

    /** Globally unique, incremental index of the test. */
    int getIndex() const;

    // TODO(darius98): Make this private!
    Executable body;

 private:
    GroupPtr group;
    int index;
};

}

#endif
